/*
 *   libcloudos
 *   Copyright (C) 2014  Tony Wolf <wolf@os-forge.net>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; version 2 of the License.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/



#include <boost/foreach.hpp>
#include <boost/regex.hpp>

#include <cloudos/system/Directory.hpp>

namespace cloudos {
namespace system {
  
  Directory::Directory ( const fs::path& p_dir ) : c_directory(p_dir) {
    c_check_cache = -1;
  }
  
  Directory::Directory ( const Directory& p_from_directory ) {
    c_directory   = p_from_directory.c_directory;
    c_check_cache = p_from_directory.c_check_cache;
    c_is_valid    = p_from_directory.c_is_valid;
  }
  
  Directory& Directory::operator= ( Directory && p_from_directory ) {
    c_directory   = std::move(p_from_directory.c_directory);
    c_check_cache = std::move(p_from_directory.c_check_cache);
    c_is_valid    = std::move(p_from_directory.c_is_valid);
    return *this;
  }
  
  Directory& Directory::operator= ( const Directory& p_from_directory ) {
    c_directory   = p_from_directory.c_directory;
    c_check_cache = p_from_directory.c_check_cache;
    c_is_valid    = p_from_directory.c_is_valid;
    return *this;
  }
  
  int Directory::createIfNotExists() {
    
    if( c_check_cache >= 0 ) {
      LOG_T() << "step over check; using cache result...";
      return c_check_cache; // we already checked this directory, return cached result
    }
    
    LOG_I() << "check directory " << c_directory.string();
    
    if( fs::exists( c_directory ) == false ) {
      LOG_I() << "does not exists, try to create it...";
      
      if( fs::create_directories( c_directory ) ) {
        LOG_I() << "directory successful created";
        c_check_cache = 0;
        c_is_valid = true;
      } else {
        LOG_E() << "could not create directory :-( Abort!";
        c_check_cache = 1;
      }
    } else if( fs::is_directory( c_directory ) == false ) {
      LOG_E() << "The given directory ( " << c_directory.string() << " ) is not a directory! Abort!";
      c_check_cache = 4;
    } else {
      c_check_cache = 0;
      c_is_valid = true;
    }
    
    return c_check_cache;
  }
  
  bool Directory::isAccessible() {
    if( createIfNotExists() != 0 ) {
      return false;
    }
    
    if( access( c_directory.c_str(), R_OK | X_OK ) != 0 ) {
     LOG_W() << "The directory is not accessible (not enough rights for access, need Read/Exec)";
     return false;
    }
    
    return true;
  }
  
  bool Directory::isWriteable() {
    if( createIfNotExists() != 0 ) {
      return false;
    }
    
    if( access( c_directory.c_str(), R_OK | W_OK | X_OK ) != 0 ) {
      LOG_W() << "The directory is not writeable (not enough rights for access, need Read/Write/Exec)";
      return false;
    }
    
    return true;
  }
  
  fs::path Directory::getDirectory() const {
    return c_directory;
  }
  
  bool Directory::findFiles( std::vector< fs::path >& p_storage, const std::string& p_phrase, ushort p_mode, ushort p_type, uint p_recursion_level ) {
    LOG_D() << "try to find files for directory: " << c_directory.string()
            << " and phrase: " << p_phrase << " type: " << p_type << " mode: " << p_mode;
    
    if( reportInvalidObjectError( __PRETTY_FUNCTION__ ) ) {
      return false;
    }

    if( p_phrase.empty() ) {
      LOG_D() << "got empty phrase to search for... Abort!";
      return false;
    }
    
    bool file_match = false;
    
    fs::directory_iterator di( c_directory ), eod; // eod == endOfDirectory
    
    BOOST_FOREACH( const fs::path& file, std::make_pair(di,eod) ) {
      bool file_type_match = false;
      
      if( fs::is_directory( file ) ) {
        file_type_match = p_type & FILE_TYPE_DIRECTORY;
        
        if( p_recursion_level > 0 ) {
          c_directory = file;
          file_match = findFiles( p_storage, p_phrase, p_mode, p_type, p_recursion_level - 1 ) || file_match;
          c_directory = file.parent_path();
        }
      }
      file_type_match = ( p_type & FILE_TYPE_REGULAR && fs::is_regular_file( file ) ) || file_type_match;
      file_type_match = ( p_type & FILE_TYPE_SYMLINK && fs::is_symlink( file ) ) || file_type_match;
      
      if( file_type_match ) {
        std::string filename = file.filename().string();
        std::size_t phrase_length   = p_phrase.length();
        std::size_t filename_length = filename.length();
        //LOG_T() << "file type matches, checking name... got " << filename << " looking for " << p_phrase;
        
        // if we match against a string, at least, it has to be sized equaly
        if( p_mode & LOOKUP_BY_REGEX || ( p_mode > LOOKUP_BY_REGEX && filename_length >= phrase_length ) ) {
          
          if( ( p_mode & LOOKUP_BY_FULLMATCH  && ( filename == p_phrase ) )                               ||
              ( p_mode & LOOKUP_BY_STARTSWITH && ( filename.substr(0, phrase_length) == p_phrase ) )      ||
              ( p_mode & LOOKUP_BY_REGEX      && boost::regex_match(filename, boost::regex( p_phrase )) ) ||
              ( p_mode & LOOKUP_BY_ENDSWITH   && ( filename.substr( filename_length - phrase_length ) == p_phrase ) ) ) {
            p_storage.push_back( file );
            file_match = true;
            LOG_D() << "found file: " << file.string();
            
            // check again, as we migth be able to imrove performance here...
            // on full match, there could be just one entry in one directory level
            if( p_recursion_level == 0 && p_mode & LOOKUP_BY_FULLMATCH  && ( filename == p_phrase ) ) {
              return file_match;
            }
          }
          
        } // if LOOKUP_BY_REGEX xor...
      } // if file_type_match
    } // BOOST_FOREACH

    return file_match;
  }
  
}}
