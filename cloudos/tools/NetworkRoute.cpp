
#include "NetworkRoute.hpp"

namespace cloudos {
namespace tools {
  
  NetworkRoute::NetworkRoute() {
    c_settings = NetworkRouteConfigPointer( new config::os::NetworkRoute );
  }
  
  bool NetworkRoute::initFromSystem() {
    return false;
  }
  
  bool NetworkRoute::setSettings ( const fs::path& p_file ) {
    if( fs::exists( p_file ) == false ) {
      return false;
    }
    return tools::System::readMessage(p_file, c_settings);
  }
  
  bool NetworkRoute::setSettings ( NetworkRouteConfigPointer p_settings ) {
    c_settings->CopyFrom( *p_settings );
    return true;
  }
  
  bool NetworkRoute::setSettings ( config::os::NetworkRoute* p_config ) {
    NetworkRouteConfigPointer config = NetworkRouteConfigPointer( p_config->New() );
    config->CopyFrom( *p_config );
    return setSettings( config );
  }

  NetworkRouteConfigPointer NetworkRoute::getSettings() {
    NetworkRouteConfigPointer msg( c_settings->New() );
    msg->CopyFrom( *c_settings );
    return msg;
  }

  bool NetworkRoute::applyToSystem() {
    /*
    std::string route_type_string = "via";
    std::string route_gate = c_route_via_ip->to_string();
    
    if( c_settings->type() == config::os::NetworkRoute_Type_DEV ) { // reset our route_type_string, if required
      route_type_string = "dev";
      route_gate = c_route_dev_name;
    }
    
    std::vector<std::string> args(6);
    args[0] = ""; // the first one, needs to be empty
    args[1] = "route";
    args[2] = "add";
    args[3] = ""; // will be set later
    args[4] = route_type_string;
    args[5] = route_gate;
    
    std::set<IPAddressPointer>::iterator *i;
    for(i = c_route_ip.begin(); i != c_route_ip.end(); ++i) {
      args[3] = i->getValue(); // set to ip address to be routed
      // exec command and go on...
    }*/
    return false;
  }

  bool NetworkRoute::removeFromSystem() {
    return false;
  }

  bool NetworkRoute::isConfigAlreadyApplied() {
    return false;
  }
  
  bool NetworkRoute::addRoute ( const IPAddress& p_route ) {
    if( p_route.isValid() == false ) {
      return false;
    }
    
    if( isRouteAlreadySet( p_route ) == false ) {
      c_settings->add_route_ip( p_route.cidr() );
    }
    return true;
  }
  
  bool NetworkRoute::setGateway ( const IPAddress& p_gateway ) {
    if( p_gateway.isValid() ) {
      c_settings->set_gateway( p_gateway.ip() );
      return true;
    }
    return false;
  }

  
  bool NetworkRoute::isRouteAlreadySet ( const IPAddress& p_ip ) const {
    const pb::RepeatedPtrField<std::string>& routes = c_settings->route_ip();
    pb::RepeatedPtrField<std::string>::const_iterator cit;
    
    for( cit = routes.begin(); cit != routes.end(); ++cit ) {
      if( p_ip == (*cit) ) {
        return true;
      }
    }
    return false;
  }

  
}} // cloudos::tools
