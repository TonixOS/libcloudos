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



#ifndef CLOUDOS_TOOLS_NETWORKROUTE_HPP__
#define CLOUDOS_TOOLS_NETWORKROUTE_HPP__

#include <cloudos/tools/NetworkElement.hpp>

namespace cloudos {
namespace tools {
  
  class NetworkRoute;
  typedef boost::shared_ptr<NetworkRoute> NetworkRoutePointer;
  typedef config::os::NetworkRoute        NetworkRouteConfig;
  
  /**
   * TODO: provide static member deduplicateRouteIPs()
   *                             applyToSystem()
   *                             removeFromSystem()
   *                             addRoute()
   */
  class NetworkRoute : public NetworkElement, public core::Config<NetworkRouteConfig> {
  public:
    NetworkRoute();
    
    /**
     * Loads all data from the currenct system settings of the route
     * TODO: to be implemented
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
     * Removes the route from the routing table, if available
     * returns true, if
     *   * the route is already removed
     *   * the route could be removed
     */
    virtual bool removeFromSystem();
    
    /**
     * returns true, if the route(s) are active in our system routing table
     */
    virtual bool isConfigAlreadyApplied();
    
    /**
     * adds a route if not already set (both will return true)
     * returns false, if the givven p_route is invalid
     */
    bool addRoute(const IPAddress& p_route);
    
    /**
     * Adds a route (if not already set) to the NetworkRoute object.
     * 
     * Will return true, if the route could be added or is already added.
     * Else false.
     */
    static bool addRoute( NetworkRouteConfig* p_config, IPAddress&& p_route );
    
    /**
     * Checks, if the given route already exists...
     */
    static bool routeExists(const NetworkRouteConfig* p_config, const IPAddress& p_route);
    
    // TODO: handle interface names too
//    bool setGateway(const ip::address& p_gateway);
    
//    bool setGateway(const std::string& p_gateway);
    
    bool setGateway(const IPAddress& p_gateway);
    
    bool isRouteAlreadySet( const IPAddress& p_ip ) const;
    
  protected:
  private:
  };
  
}} // cloudos::tools

#endif
