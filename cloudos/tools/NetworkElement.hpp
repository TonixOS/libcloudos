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



#ifndef CLOUDOS_TOOLS_NETWORKELEMENT_HPP__
#define CLOUDOS_TOOLS_NETWORKELEMENT_HPP__

#include <cloudos/core/Config.hpp>
#include <cloudos/tools/IPAddress.hpp>
#include <cloudos/proto/OS.Network.pb.h>

namespace pb = google::protobuf;

namespace cloudos {
namespace tools {
  
  /**
   * This class tries to force some class-basic infrastructure.
   * Also we try to avoid duplicate code...
   */
  class NetworkElement {
  public:
    
    /**
     * Loads all data from the currenct system settings of the route
     * TODO: to be implemented
     */
    virtual bool initFromSystem() = 0;
    
    /**
     * Will set the route(s) in the system, if not already active...
     * returns true, if
     *   * the route(s) are already set
     *   * the route(s) could be set
     */
    virtual bool applyToSystem() = 0;
    
    /**
     * Removes the route from the routing table, if available
     * returns true, if
     *   * the route is already removed
     *   * the route could be removed
     */
    virtual bool removeFromSystem() = 0;
    
    /**
     * returns true, if the route(s) are active in our system routing table
     */
    virtual bool isConfigAlreadyApplied() = 0;
  protected:
    
  private:
  };
  
}
}


#endif
