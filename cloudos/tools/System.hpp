
#ifndef CLOUDOS_TOOLS_SYSTEM_HPP__
#define CLOUDOS_TOOLS_SYSTEM_HPP__


extern "C" {
//for NIC detection
#include <net/if.h>
}

#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

namespace fs = boost::filesystem;
namespace pb = google::protobuf;

namespace cloudos {
namespace tools {
  
  typedef boost::shared_ptr<std::stringstream> StringStreamPointer;
  
  typedef boost::shared_ptr<std::fstream> FStreamPointer;
  
  typedef boost::shared_ptr<pb::Message> MessagePointer;
  
  class System {
  public:
    
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
     * will return a set of interfaces (their names), available on this system
     */
    static std::set<std::string> getAvailableInterfaces();
    
  };
  
}
}

#endif
