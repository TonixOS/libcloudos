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



#ifndef CLOUDOS_TOOLS_SYSTEM_HPP__
#define CLOUDOS_TOOLS_SYSTEM_HPP__


extern "C" {
//for NIC detection
#include <net/if.h>
} // extern "C"

#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/container/stable_vector.hpp>

#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <cloudos/system/HDDisk.hpp>
#include <cloudos/tools/NetworkInterface.hpp>

namespace fs = boost::filesystem;
namespace bc = boost::container;
namespace pb = google::protobuf;

namespace cloudos {
namespace tools {
  
  typedef boost::shared_ptr<std::stringstream> StringStreamPointer;
  typedef boost::shared_ptr<std::fstream> FStreamPointer;
  typedef boost::shared_ptr<pb::Message> MessagePointer;
  
  class System {
  public:
    
    typedef bc::stable_vector< system::HDDisk::ConfigPointer >   HDDiskConfigPointerVector;
    typedef bc::stable_vector< NetworkInterface::ConfigPointer > NICConfigPointerVector;
    
    /**
     * Will read a google::Message from file, givven by p_filename
     * returns false, if something went wrong
     * Message will be stored in p_msg.
     */
    template<typename T>
    static bool readMessage( const fs::path& p_filename, T p_msg ) {
      
      if( fs::exists( p_filename ) == false ) {
        return false;
      }  // TODO: check if file is readable
      
      FStreamPointer inputFile( new std::fstream );
      inputFile->open(p_filename.string().c_str(), std::ios::in);
      
      std::cout << "reading message file " << p_filename.string() << std::endl;
      
      return readMessage( inputFile, p_msg );
    }
    
    /**
     * reads a google::Message from input stream, givven by p_istream
     * returns false, if something went wrong
     * Message will be stored in p_msg.
     */
    template<typename T>
    static bool readMessage( FStreamPointer p_istream, T p_msg ) {
      if( p_istream->good() == false ) {
        return false;
      }
      std::stringstream ss;
      std::string line;
      
      do {
        std::getline( *p_istream , line);
        ss << line << std::endl;
        line.clear();
      } while(p_istream->good());
      
      return pb::TextFormat::ParseFromString(ss.str(), p_msg.get());
    }
    
    /**
     * writes a protocol buffers message to the givven file in p_filename
     */
    template<typename T>
    static bool writeMessage( const fs::path& p_filename, T p_msg ) {
      boost::shared_ptr<std::string> message_string;
      bool success = true;
      
      success = pb::TextFormat::PrintToString( *p_msg, message_string.get() );
      
      if(success == false) {
        return false;
      }
      
      std::fstream file;
      file.open(p_filename.string().c_str(), std::ios::out);
      
      if( file.good() == false ) {
        return false;
      }
      
      file << *message_string << std::endl;
      return true;
    }
    
    /**
     * Will split the string p_source_string by all characters, given by p_delimiters.
     * Will store all fields in p_storage (appended).
     * Empty fields will be inserted too!
     * 
     * Use it like:
     *   std::vector<std::string> storage;
     *   System::splitString("Dies;ist|;ein;test", ";|", storage);
     * Will result in:
     *   ("Dies", "ist", "", "ein", "test")
     */
    static void splitString(const std::string& p_source_string,
                            const std::string& p_delimiters,
                            std::vector<std::string>& p_storage);
    
    /**
     * will return a set of interfaces (their names), available on this system
     */
    static NICConfigPointerVector getAvailableInterfaces();
    
    /**
     * Will return a vector of available disks and their config settings...
     * 
     * If p_filter_installable is set, it will just return disk where the storage size
     * is bigger than 20GB.
     */
    static HDDiskConfigPointerVector getAvailableHDDisks( bool p_filter_installable = false );
    
    /**
     * Will register a SIGSEGV handler (our backtrace() function)
     */
    static void enableBacktrace();
    
    /**
     * Will do the backtrace
     */
    static void backtrace(int p_sig);
    
  };
  
}
}

#endif
