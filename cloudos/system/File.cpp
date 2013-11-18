

#include <fstream>

#include <cloudos/system/File.hpp>

namespace cloudos {
namespace system {
  
  File::File ( const fs::path& p_file ) {
    c_file = p_file;
    if( fs::exists( c_file ) == false ) {
      c_is_valid = true;
    }
  }

  bool File::getContent ( std::stringstream& p_storage ) {
    if( c_is_valid == false ) {
      return false;
    }
    
    std::ifstream input( c_file.string(), std::ios::in );
    if( input.is_open() == false ) {
      return false; //FIXME: Log error
    }
    
    std::string line;
    while( input.good() ) {
      std::getline(input, line);
      p_storage << line << std::endl;
      line.clear();
    }
    
    return true;
  }

  
}} // cloudos::system
