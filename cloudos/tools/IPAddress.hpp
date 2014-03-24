
#ifndef CLOUDOS_TOOLS_IPADDRESS_HPP__
#define CLOUDOS_TOOLS_IPADDRESS_HPP__

#include <boost/asio/ip/address.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

#include <cloudos/core/Object.hpp>

extern "C" {
#include <arpa/inet.h>
}

namespace ip = boost::asio::ip;

namespace cloudos {
namespace tools {
  
  class IPAddress;
  
  typedef boost::shared_ptr<IPAddress> IPAddressPointer;
  
  class IPAddress : public core::Object {
  public:
    
    IPAddress();
    IPAddress( const std::string& p_ip );
    IPAddress( const ::ip::address& p_ip );
    
    bool isValid() const override;
    
    /** 
     * @brief takes & validates an ip address as string like 127.0.0.1/24 - prefix is required!
     * Won't overwrite interal settings, unless the givven string is valid
     */
    bool setValue( const std::string& p_ip);
    
    /**
     * will increment the current IP about the givven value
     * 
     * To decrement the value, just use something like "-2"
     * 
     * e.g.: IPAddress(10.0.0.2).setIncrementValue(4) will result in 10.0.0.6
     */
    bool setIncrementValue( int p_value );
    
    /**
     * return the address in CIDR notation
     */
    std::string cidr() const;
    
    /**
     * returns the address without prefix (CIDR)
     */
    std::string ip() const;
    
    /**
     * returns the netmask (like 255.255.255.0) of the current IP address
     * return will be empty if object is invalid
     * 
     * FIXME: add support for IPv6
     */
    std::string netmask() const;
    
    /**
     * returns the broadcast address (like 192.168.255.255) of the current IP address
     * return will be empty if object is invalid
     * 
     * FIXME: add support for IPv6
     */
    std::string broadcast() const;
    
    /**
     * returns the netaddress of the current IP address
     * return will be empty if object is invalid
     * 
     * FIXME: add support for IPv6
     */
    std::string netaddress() const;
    
    /**
     * returns the current prefix as string (just the number, as string)
     * 
     * e.g. on 10.0.0.1/24 it will return 24
     */
    std::string prefix() const;
    
    /**
     * checks, if the givven ip address is in range of the ip-range from the current IP
     * 
     * returns:
     *   -1 => if the given IP is lower and out of scope ( e.g. 10.10.10.255/24(givven) on 10.10.11.1/24(set) )
     *    0 => if the given IP is in range
     *    1 => if the given IP is greater and out of scope ( e.g. 10.10.12.1/24(givven) on 10.10.11.1/24(set) )
     */
    int8_t isInRange( const IPAddress& p_ip );
    
    std::string error_message() { return c_error_message.str(); }
    
    bool operator==( const IPAddress& p_ip ) const;
    bool operator==( const std::string& p_ip ) const;
    
  protected:
  private:
    std::stringstream c_error_message;
    
    ::ip::address     c_address;
    
    /*
     * in host byte order
     */
    uint32_t          c_address_binary;
    std::string       c_prefix;
    uint8_t           c_prefix_number;
    
    /*
     * in host byte order
     */
    uint32_t          c_netmask;
    /*
     * in host byte order
     */
    uint32_t          c_netaddress;
    /*
     * in host byte order
     */
    uint32_t          c_broadcast;
    
    /**
     * just do the constructor stuff
     */
    void init();
    
    /**
     * will set c_netmask
     */
    void calculateNetmask();
    
    /**
     * will set c_netaddress and c_address_binary
     */
    void calculateNetaddress();
    
    /**
     * will set c_broadcast
     */
    void calculateBroadcast();
    
    /**
     * will set c_prefix_number and return, if the prefix is valid
     */
    bool isPrefixValid( const std::string& p_prefix, const ::ip::address& p_address );
  };  
  
  
}} // cloudos::tools


#endif