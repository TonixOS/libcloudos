
#ifndef CLOUDOS_TOOLS_IPADDRESS_HPP__
#define CLOUDOS_TOOLS_IPADDRESS_HPP__

#include <boost/asio/ip/address.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

namespace ip = boost::asio::ip;

namespace cloudos {
namespace tools {
  
  class IPAddress;
  
  typedef boost::shared_ptr<IPAddress> IPAddressPointer;
  
  class IPAddress {
  public:
    
    IPAddress();
    IPAddress( const std::string& p_ip );
    IPAddress( const ::ip::address& p_ip );
    
    bool isValid() const;
    
    /** 
     * @brief takes & validates an ip address as string like 127.0.0.1/24 - prefix is required!
     * Won't overwrite interal settings, unless the givven string is valid
     */
    bool setValue( const std::string& p_ip);
    
    /**
     * return the address in CIDR notation
     */
    std::string cidr() const;
    
    /**
     * returns the address withour prefix (CIDR)
     */
    std::string ip() const;
    
    std::string error_message() { return c_error_message.str(); }
    
    bool operator==( const IPAddress& p_ip ) const;
    bool operator==( const std::string& p_ip ) const;
    
    bool isPrefixValid( const std::string& p_prefix, const ::ip::address& p_address ) const;
    
  protected:
  private:
    bool c_is_valid;
    std::stringstream c_error_message;
    
    ::ip::address c_address;
    std::string              c_prefix;
  };  
  
  
}} // cloudos::tools


#endif