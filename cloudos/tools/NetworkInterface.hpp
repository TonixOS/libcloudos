
#ifndef CLOUDOS_TOOLS_NETWROKINTERFACE_HPP__
#define CLOUDOS_TOOLS_NETWROKINTERFACE_HPP__

#include <cloudos/tools/NetworkElement.hpp>
#include <cloudos/tools/NetworkRoute.hpp>

namespace cloudos {
namespace tools {
  
  class NetworkInterface;
  class NetworkRoute;
  typedef boost::shared_ptr<NetworkRoute> NetworkRoutePointer;
  
  /**
   * the first of "pair" is the NIC System ID
   * the second the NIC System name
   */
  typedef std::vector<std::pair<unsigned int, std::string> > NetworkInterfaceVector;
  
  typedef boost::shared_ptr<NetworkInterface>             NetworkInterfacePointer;
  typedef boost::shared_ptr<config::os::NetworkInterface> NetworkInterfaceConfigPointer;
  
  class NetworkInterface : public NetworkElement {
  public:
    NetworkInterface();
    NetworkInterface( const std::string& p_interface_name );
    
    /**
     * Loads all data from the currenct system settings of the interface
     * TODO: implement
     */
    virtual bool initFromSystem();
    
    virtual bool setSettings( const fs::path& p_file );
    virtual bool setSettings( NetworkInterfaceConfigPointer p_settings );
    
    virtual NetworkInterfaceConfigPointer getSettings();
    
    /**
     * Will set the route(s) in the system, if not already active...
     * returns true, if
     *   * the route(s) are already set
     *   * the route(s) could be set
     */
    virtual bool applyToSystem();
    
    /**
     * Removes the interface settings in our system, if available
     * returns true, if
     *   * the settings are already removed
     *   * the settings could be removed
     */
    virtual bool removeFromSystem();
    
    /**
     * returns true, if the interface settings are active in our system
     */
    virtual bool isConfigAlreadyApplied();
    
    /**
     * you may add multiple IPs at once with this template-based function
     * be aware, it requires T to not be a smart-pointer!
     * T also needs:
     *   - a const_iterator
     *   - cbegin()
     *   - cend()
     * Ts containment has to to an
     *   - IPAddress or
     *   - boost::asio::ip::address or
     *   - std::string
     * 
     * will return the number of newly added IPs
     * will dedup givven IPs
     */
    template<typename T>
    unsigned addIPAddresses( const T& p_container ) {
      unsigned int counter = 0;
      typename T::const_iterator i;
      for( i = p_container.cbegin(); i != p_container.cend(); ++i ) {
        IPAddress ip;
        ip.setValue( *i );
        if( ip.isValid() && hasIP( ip ) > -1 ) {
          ++counter;
          addIP( ip );
        }
      }
      return counter;
    }
    
    bool addIP( const IPAddress& p_ip );
    
    bool removeIP( const IPAddress& p_ip );
    
    bool addRoute( const IPAddress& p_route, const IPAddress& p_gateway );
    
    NetworkRoutePointer getRouteByGateway( const IPAddress& p_gateway_ip );
    
    /**
     * an easy way to call getRouteByGateway()
     */
    NetworkRoutePointer operator[]( const IPAddress& p_gateway_ip );
    
    bool hasRoute( const IPAddress& p_route );
    
    /**
     * returns -1 if the IP wasn't found in our list
     * else will return the index value, where the IP was found
     */
    int hasIP( const IPAddress& p_ip ) const;
    
  protected:
  private:
    std::string                   c_interface_name;
    std::set<NetworkRoutePointer> c_routes;
    
    NetworkInterfaceConfigPointer c_settings;
    
    /**
     * returns pointer to oject (from c_routes), or nullptr
     */
    NetworkRoutePointer isGatewayAlreadySet( const IPAddress& p_ip );
    
    /**
     * Extract all config::os::NetworkRoute objects from c_settings
     * and create an tools::NetworkRoute object from it. Insert this
     * object into c_routes
     * 
     * Will dedup routes/gateways within our c_settings object
     * 
     * Will remove all config::os::NetworkRoute objects from c_settings
     * 
     * This needs to be done to be sure, that we are in sync with c_routes
     */
    void extractRoutesFromSettings();
    
  };
  
}
}

#endif
