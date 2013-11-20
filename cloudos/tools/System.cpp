
#include <boost/tokenizer.hpp>

#include "System.hpp"

namespace cloudos {
namespace tools {
  
  void System::splitString ( const std::string& p_source_string,
                             const std::string& p_delimiters,
                             std::vector<std::string>& p_storage ) {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    
    boost::char_separator<char> sep(p_delimiters.c_str(), std::string("").c_str(), boost::keep_empty_tokens);
    tokenizer fields(p_source_string, sep);
    for(tokenizer::const_iterator it = fields.begin(); it != fields.end(); ++it) {
      p_storage.push_back( *it );
    }
  }
  
  std::set<std::string> System::getAvailableInterfaces() {
    struct if_nameindex *if_nic, *i;
    std::set<std::string> ifaces;
    
    if_nic = if_nameindex();
    if(if_nic != NULL) {
      for (i = if_nic; i->if_index != 0 && i->if_name != NULL; ++i) {
        if( std::string("lo").compare( i->if_name ) != 0 && std::string("bond0").compare( i->if_name ) != 0 ) {
          std::string iface = i->if_name;
          ifaces.insert( iface );
        }
      }
    }
    return ifaces;
  }

  
}
}
