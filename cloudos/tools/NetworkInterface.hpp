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



#ifndef CLOUDOS_TOOLS_NETWROKINTERFACE_HPP__
#define CLOUDOS_TOOLS_NETWROKINTERFACE_HPP__

#include <string>
#include <vector>

#include <cloudos/tools/NetworkElement.hpp>
#include <cloudos/tools/NetworkRoute.hpp>

namespace cloudos {
namespace tools {
  
  class NetworkInterface;
  typedef config::os::NetworkInterface NetworkInterfaceConfig;
  
  /**
   * the first of "pair" is the NIC System ID
   * the second the NIC System name
   */
  typedef std::vector<std::pair<unsigned int, std::string> > NetworkInterfaceVector;
  typedef boost::shared_ptr<NetworkInterface>                NetworkInterfacePointer;
  
  /**
   * FIXME: add comments about the class
   */
  class NetworkInterface : public NetworkElement, public core::Config<NetworkInterfaceConfig> {
  public:
    NetworkInterface();
    
    /**
     * Loads all data from the currenct system settings of the interface
     * TODO: implement
     */
    virtual bool initFromSystem();
    
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
     * Returns true, if the given IP could be removed or is not available...
     * else false.
     */
    static bool removeIP( NetworkInterfaceConfig* p_config, IPAddress&& p_ip );
    
    /**
     * Will add the ip address to p_config if it is not already in its IP list.
     * 
     * Returns true if the ip address already exists or could be added.
     */
    static bool addIP( NetworkInterfaceConfig* p_config, IPAddress&& p_ip );
    
    /**
     * Checks, if the given IPAddress is already set within p_config.
     */
    static bool ipExists( NetworkInterfaceConfig* p_config, const IPAddress& p_ip );
    
    /**
     * Adds the route, if it not already exists... will return true on success and if it already exists...
     * else false.
     */
    static bool addRoute( NetworkInterfaceConfig* p_config, IPAddress&& p_route, const IPAddress& p_gateway );
    
    /**
     * Will check, if we already have a route, where we are routing via the given p_gateway;
     * if so, return the route config object.
     * 
     * Else, create a new route config object and return it.
     */
    static NetworkRouteConfig* getRouteConfigObject( NetworkInterfaceConfig* p_config, const IPAddress& p_gateway );
    
    /**
     * Checks, if the given gateway exists in p_config->route() repeated field.
     * 
     * Returns the corresponding NetworkRouteConfig* or a nullptr, if the gateway
     * does not exist.
     */
    static NetworkRouteConfig* gatewayExists( NetworkInterfaceConfig* p_config, const IPAddress& p_gateway );
    
  protected:
  private:
  };
  
}
}

#endif
