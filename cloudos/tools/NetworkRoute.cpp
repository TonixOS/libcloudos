
#include <string>

#include "NetworkRoute.hpp"

namespace blog = boost::log;

namespace cloudos {
namespace tools {
  
  NetworkRoute::NetworkRoute() {
  }
  
  bool NetworkRoute::initFromSystem() {
    return false; // TODO: implement initFromSystem()
  }

  bool NetworkRoute::applyToSystem() {
    // TODO: implement applyToSystem()
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
    return false;// TODO: implement removeFromSystem()
  }

  bool NetworkRoute::isConfigAlreadyApplied() {
    return false;// TODO: implement isConfigAlreadyApplied()
  }
  
  bool NetworkRoute::addRoute ( const IPAddress& p_route ) {
    LOG_D() << "adding route " << p_route.cidr();
    if( p_route.isValid() == false ) {
      LOG_E() << "could not add route, as the IPAddress is invalid... Abort!";
      return false;
    }
    
    if( isRouteAlreadySet( p_route ) == false ) {
      c_config->add_route_ip( p_route.cidr() );
    }
    return true;
  }
  
  bool NetworkRoute::addRoute ( NetworkRouteConfig* p_config, IPAddress && p_route ) {
    Logger::LogSourceType lg;
    
    if( p_config == nullptr ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "could not check route, p_config is nullptr...";
      return false;
    }
    
    if( p_route.isValid() == false ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "could not add route, as the IPAddress is invalid... Abort!";
      return false;
    }
    
    BOOST_LOG_SEV(lg, blog::trivial::debug) << "adding route " << p_route.cidr();
    
    if( NetworkRoute::routeExists( p_config, p_route ) ) {
      BOOST_LOG_SEV(lg, blog::trivial::debug) << "route already exists...";
      return true;
    }
    
    p_config->add_route_ip( p_route.cidr() );
    BOOST_LOG_SEV(lg, blog::trivial::debug) << "route added";
    return true;
  }
  
  bool NetworkRoute::routeExists ( const NetworkRouteConfig* p_config, const IPAddress& p_route ) {
    Logger::LogSourceType lg;
    if( p_config == nullptr ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "could not check route, p_config is nullptr...";
      return false;
    }
    if( p_route.isValid() == false ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "could not check route, as the IPAddress is invalid... Abort!";
      return false;
    }
    
    std::string route( p_route.cidr() );
    for( auto& route2 : p_config->route_ip() ) {
      if( route == route2 ) {
        return true;
      }
    }
    return false;
  }
  
  bool NetworkRoute::setGateway ( const IPAddress& p_gateway ) {
    LOG_D() << "setting gateway... " << p_gateway.ip();
    
    if( p_gateway.isValid() == false ) {
      LOG_E() << "failed, as gateway IP is invalid... Abort!";
      return false;
    }
    
    c_config->set_index( p_gateway.ip() );
    return true;
  }

  
  bool NetworkRoute::isRouteAlreadySet ( const IPAddress& p_ip ) const {
    //FIXME: use C++11 auto for()
    const pb::RepeatedPtrField<std::string>& routes = c_config->route_ip();
    pb::RepeatedPtrField<std::string>::const_iterator cit;
    
    for( cit = routes.begin(); cit != routes.end(); ++cit ) {
      if( p_ip == (*cit) ) {
        return true;
      }
    }
    return false;
  }

  
}} // cloudos::tools
