
#include "System.hpp"

namespace cloudos {
namespace tools {
  
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
