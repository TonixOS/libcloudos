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



// for access() call
extern "C" {
#include <unistd.h>
}

#include <fstream>

#include <cloudos/system/File.hpp>

namespace cloudos {
namespace system {
  
  File::File ( fs::path && p_file ) {
    c_file_str = p_file.string();
    c_file = std::move( p_file );
    c_check_cache = -1;
    LOG_T() << "set file: " << c_file_str;
    
    checkIsValid();
  }
  
  File::File ( const fs::path& p_file ) {
    c_file = p_file;
    c_file_str = c_file.string();
    c_check_cache = -1;
    
    checkIsValid();
  }
  
  ushort File::createIfNotExists() {
    if( c_check_cache >= 0 ) {
      LOG_T() << "step over check; using cache result...";
      return c_check_cache; // we already checked this file, return cached result
    }
    
    LOG_I() << "check file " << c_file_str;
    
    if( fs::exists( c_file ) == false ) {
      LOG_I() << "does not exists, check if creating the file will work...";
      
      Directory parent_path( c_file.parent_path() );
      if( parent_path.isWriteable() == false ) {
        LOG_E() << "parent path is not writeable, so creating the file will fail... Abort";
        c_check_cache = 6;
        return c_check_cache;
      }
      
      // create the file
      std::ofstream out( c_file_str, std::ios::app );
      if( out.is_open() == false ) {
        LOG_E() << "creating the file failed... Abort!";
        c_check_cache = 1;
        return c_check_cache;
      }
      
    }
    
    if( fs::is_regular_file( c_file ) == false && fs::is_symlink( c_file ) == false ) {
      LOG_E() << "path is not a file! Abort";
      c_check_cache = 4;
      return c_check_cache;
    }
    
    c_check_cache = 0;
    
    return c_check_cache;
  }
  
  bool File::isAccessible() {
    if( createIfNotExists() != 0 ) {
      return false;
    }
    
    if( access( c_file_str.c_str(), R_OK ) != 0 ) {
      LOG_W() << "The file is not accessible (not enough rights for access, need Read)";
      return false;
    }
    
    return true;
  }
    
  bool File::isWriteable() {
    if( createIfNotExists() != 0 ) {
      return false;
    }
    
    if( access( c_file_str.c_str(), R_OK | W_OK ) != 0 ) {
      LOG_W() << "The file is not writeable (not enough rights for access, need Read/Write)";
      return false;
    }
    
    return true;
  }

  bool File::getContent ( std::stringstream& p_storage ) {
    LOG_D() << "read content from file: " << c_file_str;
    
    if( isAccessible() == false ) {
      LOG_E() << "file is not readable! Abort!";
      return false;
    }
    
    LOG_D() << "opening file " << c_file_str;
    
    std::ifstream input( c_file_str, std::ios::in );
    if( input.is_open() == false ) {
      LOG_E() << "Failed to open file: " << c_file_str;
      return false;
    }
    
    LOG_D() << "start reading content of file " << c_file_str;
    
    std::string line;
    while( std::getline(input, line) ) {
      p_storage << line << std::endl;
    }
    
    LOG_D() << "reading content done";
    
    return true;
  }
  
  bool File::setContent ( std::stringstream && p_new_content ) {
    LOG_D() << "truncate file and write new content to it: " << c_file_str;
    
    if( isWriteable() == false ) {
      LOG_E() << "file is not writeable! Abort!";
      return false;
    }
    
    std::ofstream output( c_file_str, std::ios::trunc );
    return insertContent( output, std::move( p_new_content ) );
  }
  
  bool File::setContent ( std::string && p_new_content ) {
    LOG_D() << "truncate file and write new content to it: " << c_file_str;
    
    if( isWriteable() == false ) {
      LOG_E() << "file is not writeable! Abort!";
      return false;
    }
    
    std::ofstream output( c_file_str, std::ios::trunc );
    return insertContent( output, std::move( p_new_content ) );
  }
  
  bool File::addContent ( std::stringstream && p_add_content ) {
    LOG_D() << "append content to file: " << c_file_str;
    
    if( isWriteable() == false ) {
      LOG_E() << "file is not writeable! Abort!";
      return false;
    }
    
    std::ofstream output( c_file_str, std::ios::app );
    return insertContent( output, std::move( p_add_content ) );
  }
  
  // 
  // P R I V A T E
  // 
  bool File::insertContent ( std::ofstream& p_stream, std::stringstream && p_content ) {
    if( p_stream.is_open() == false ) {
      LOG_E() << "opening the file " << c_file_str << " failed; abort!";
      return false;
    }
    
    p_stream << p_content.str();
    p_stream.flush();
    if( p_stream.bad() ) {
      LOG_W() << "something happend to the file, while writing our data to it... abort!";
      return false;
    }
    
    return true;
  }
  
  bool File::insertContent ( std::ofstream& p_stream, std::string && p_content ) {
    if( p_stream.is_open() == false ) {
      LOG_E() << "opening the file " << c_file_str << " failed; abort!";
      return false;
    }
    
    p_stream << p_content;
    p_stream.flush();
    if( p_stream.bad() ) {
      LOG_W() << "something happend to the file, while writing our data to it... abort!";
      return false;
    }
    
    return true;
  }
  
  void File::setRequireExists ( bool p_value ) {
    c_requires_file_exists = p_value;
    
    // our requirements changed, so check again, if we are valid
    checkIsValid();
  }
  
  bool File::filenameEndsWith ( std::string && p_lookup ) const {
    return c_file_str.substr( c_file_str.length() - p_lookup.length() ) == p_lookup;
  }
  
  // 
  // P R O T E C T E D
  // 
  
  void File::checkIsValid() {
    if( c_file_str.empty() ) {
      c_is_valid = false;
      return;
    }
    
    if( c_requires_file_exists && (fs::exists( c_file ) == false ||  access(c_file_str.c_str(), R_OK) != 0) ) {
      c_is_valid = false;
      return;
    }
    
    c_is_valid = true;
  }

  
}} // cloudos::system
