
#include <cloudos/tools/IPAddress.hpp>

namespace cloudos {
namespace tools {
  
  IPAddress::IPAddress() {
    c_error_message.clear();
    c_is_valid = false;
  }
  
  IPAddress::IPAddress ( const std::string& p_ip ) {
    c_error_message.clear();
    c_is_valid = false;
    setValue( p_ip );
  }
  
  IPAddress::IPAddress ( const ::ip::address& p_ip ) {
    c_error_message.clear();
    c_is_valid = false;
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
    } else {
      address = ip::address::from_string( p_ip.substr(0, prefix_pos), error_address );
      prefix  = p_ip.substr(prefix_pos+1);
      if( isPrefixValid( prefix, address ) == false ) {
        c_error_message << "invalid ip prefix givven: " << p_ip << std::endl;
        return false;
      }
    }
    
    if( error_address ) {
      c_error_message << "invalid IP address givven: " << p_ip << std::endl;
      return false;
    }
    
    c_address  = address;
    c_prefix   = prefix;
    c_is_valid = true;
    return true;
  }
  
  std::string IPAddress::cidr() const {
    return c_address.to_string() + '/' + c_prefix;
  }
  
  std::string IPAddress::ip() const {
    return c_address.to_string();
  }
  
  bool IPAddress::operator== ( const IPAddress& p_ip ) const {
    return cidr() == p_ip.cidr();
  }
  
  bool IPAddress::operator== ( const std::string& p_ip ) const {
    return cidr() == p_ip;
  }

  bool IPAddress::isPrefixValid ( const std::string& p_prefix, const ::ip::address& p_address ) const {
    int prefix = -1;
    uint8_t max_prefix = (p_address.is_v4()) ? 32 : 128;
    
    try {
      prefix = boost::lexical_cast<int>(p_prefix);
    } catch(boost::bad_lexical_cast &e) {
      return false;
    }   
    
    return ( prefix >= 0 && prefix < max_prefix );
  }

}} // cloudos::tools
