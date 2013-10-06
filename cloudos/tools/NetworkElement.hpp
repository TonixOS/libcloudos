
#ifndef CLOUDOS_TOOLS_NETWORKELEMENT_HPP__
#define CLOUDOS_TOOLS_NETWORKELEMENT_HPP__

#include <set>
#include <string>
#include <fstream>
#include <vector>

#include <boost/process/process.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/filesystem.hpp>

#include <google/protobuf/message.h>

#include <cloudos/tools/System.hpp>
#include <cloudos/tools/IPAddress.hpp>

#include <cloudos/proto/OS.Network.pb.h>

namespace ip = boost::asio::ip;
namespace pb = google::protobuf;

namespace cloudos {
namespace tools {
  
  /**
   * This class tries to force some class-basic infrastructure.
   * Also we try to avoid duplicate code...
   */
  class NetworkElement {
  public:
    
    virtual bool setSettings( const fs::path& p_file ) = 0;
    
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
