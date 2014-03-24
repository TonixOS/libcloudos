
#include <cloudos/tools/IPAddress.hpp>

namespace cloudos {
namespace tools {
  
  IPAddress::IPAddress() {
    init();
  }
  
  IPAddress::IPAddress ( const std::string& p_ip ) {
    init();
    setValue( p_ip );
  }
  
  IPAddress::IPAddress ( const ::ip::address& p_ip ) {
    init();
    setValue( p_ip.to_string() );
  }
  
  bool IPAddress::isValid() const {
    return c_is_valid;
  }

  bool IPAddress::setValue ( const std::string& p_ip ) { 
    boost::system::error_code error_address;
    ::ip::address address;
    std::string prefix;
    // our IP string object needs its prefix
    std::size_t prefix_pos = p_ip.rfind('/');
    
    if( prefix_pos == std::string::npos ) {
      address = ip::address::from_string( p_ip, error_address );
      prefix = address.is_v4() ? "32" : "128";
      c_prefix_number = address.is_v4() ? 32 : 128;
    } else {
      address = ip::address::from_string( p_ip.substr(0, prefix_pos), error_address );
      prefix  = p_ip.substr(prefix_pos+1);
      if( isPrefixValid( prefix, address ) == false ) {
        LOG_W() << "invalid ip prefix given in string: " << p_ip;
        c_error_message << "invalid ip prefix givven: " << p_ip << std::endl;
        return false;
      }
    }
    
    if( error_address ) {
      LOG_W() << "invalid ip address given in string: " << p_ip;
      c_error_message << "invalid IP address givven: " << p_ip << std::endl;
      return false;
    }
    
    // set basics
    c_address  = address;
    c_prefix   = prefix;
    
    calculateNetmask();
    calculateNetaddress();
    calculateBroadcast();
    
    c_is_valid = true;
    return true;
  }
  
  bool IPAddress::setIncrementValue ( int p_value ) {
    c_address_binary += p_value;
    
    in_addr ip_in_addr;
    ip_in_addr.s_addr = htonl( c_address_binary );
    
    std::string ip_str( inet_ntoa( ip_in_addr ) );
    c_address = ip::address::from_string( ip_str );
    
    calculateNetaddress();
    calculateBroadcast();
    return true;
  }
  
  std::string IPAddress::cidr() const {
    return c_address.to_string() + '/' + c_prefix;
  }
  
  std::string IPAddress::ip() const {
    return c_address.to_string();
  }
  
  std::string IPAddress::netmask() const {
    if( c_is_valid == false ) {
      return std::string();
    }
    
    in_addr tmp;
    tmp.s_addr = htonl( c_netmask );
    return std::string( inet_ntoa( tmp ) );
  }

  std::string IPAddress::netaddress() const {
    if( c_is_valid == false ) {
      return std::string();
    }
    
    in_addr tmp;
    tmp.s_addr = htonl( c_netaddress );
    return std::string( inet_ntoa( tmp ) );
  }
  
  std::string IPAddress::prefix() const {
    if( c_is_valid == false ) {
      return std::string();
    }
    return c_prefix;
  }

  
  std::string IPAddress::broadcast() const {
    if( c_is_valid == false ) {
      return std::string();
    }
    
    in_addr tmp;
    tmp.s_addr = htonl( c_broadcast );
    return std::string( inet_ntoa( tmp ) );
  }

  int8_t IPAddress::isInRange( const IPAddress& p_ip ) {
    
    if( p_ip.c_address_binary < c_netaddress ) {
      return -1;
    }
    if( p_ip.c_address_binary > c_broadcast ) {
      return 1;
    }
    return 0;
  }
  
  bool IPAddress::operator== ( const IPAddress& p_ip ) const {
    return cidr() == p_ip.cidr();
  }
  
  bool IPAddress::operator== ( const std::string& p_ip ) const {
    return cidr() == p_ip;
  }

  bool IPAddress::isPrefixValid ( const std::string& p_prefix, const ::ip::address& p_address ) {
    uint8_t max_prefix = p_address.is_v4() ? 32 : 128;
    
    try {
      c_prefix_number = boost::lexical_cast<int>(p_prefix);
    } catch(boost::bad_lexical_cast &e) {
      return false;
    }
    
    return ( c_prefix_number >= 0 && c_prefix_number < max_prefix );
  }
  
  // 
  // P R I V A T E
  // 
  void IPAddress::init() {
    c_error_message.clear();
    c_netmask    =     0;
    c_broadcast  =     0;
    c_netaddress =     0;
    c_prefix_number  = 0;
    c_address_binary = 0;
  }

  void IPAddress::calculateNetmask() {
    uint32_t mask = ~uint32_t(0);
    for(int i = 0; i < c_prefix_number; ++i) {
      mask = mask >> 1;
    }
    c_netmask = ~mask;
  }

  void IPAddress::calculateNetaddress() {
    // set netaddress
    // FIXME add support for IPv6
    in_addr in_addr_ip;
    inet_aton( c_address.to_string().c_str(), &in_addr_ip );
    c_address_binary = ntohl( in_addr_ip.s_addr );
    c_netaddress = ( c_address_binary & c_netmask );
  }
  
  void IPAddress::calculateBroadcast() {
    // set broadcast address
    // FIXME add support for IPv6
    c_broadcast = ( c_address_binary | ~c_netmask );
  }



}} // cloudos::tools
