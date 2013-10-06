
#include <cloudos/tools/NetworkInterface.hpp>
#include <cloudos/tools/NetworkRoute.hpp>

namespace cloudos {
namespace tools {
  
  NetworkInterface::NetworkInterface() {
    c_settings = NetworkInterfaceConfigPointer( new config::os::NetworkInterface );
  }
  
  NetworkInterface::NetworkInterface ( const std::string& p_interface_name ) {
    c_interface_name = p_interface_name;
    c_settings = NetworkInterfaceConfigPointer( new config::os::NetworkInterface );
  }
  
  bool NetworkInterface::setSettings ( const fs::path& p_file ) {
    if( fs::exists( p_file ) == false ) {
      return false;
    }
    if( tools::System::readMessage(p_file, c_settings) == false ) {
      return false;
    }
    extractRoutesFromSettings();
    return true;
  }

  bool NetworkInterface::setSettings ( NetworkInterfaceConfigPointer p_settings ) {
    c_settings->CopyFrom( *p_settings );
    extractRoutesFromSettings();
    return true;
  }

  NetworkInterfaceConfigPointer NetworkInterface::getSettings() {
    NetworkInterfaceConfigPointer msg( c_settings->New() );
    msg->CopyFrom( *c_settings );
    
    // our internal config object does not contain any route config objects
    // we outsourced them to tools::NetworkRoute config controller
    // therefor: take them back and insert them into our interface config object
    std::set<NetworkRoutePointer>::iterator it;
    for( it = c_routes.begin(); it != c_routes.end(); ++it ) {
      msg->add_route()->CopyFrom( *(it->get()->getSettings()) );
    }
    
    return msg;
  }
  
  bool NetworkInterface::applyToSystem() {
    return false;
  }
  
  bool NetworkInterface::initFromSystem() {
    return false;
  }
  
  bool NetworkInterface::removeFromSystem() {
    return false;
  }

  bool NetworkInterface::isConfigAlreadyApplied() {
    return false;
  }

  bool NetworkInterface::addIP ( const IPAddress& p_ip ) {
    if( p_ip.isValid() == false ) {
      return false;
    }
    c_settings->add_ip_cidr( p_ip.cidr() );
    return true;
  }
  
  bool NetworkInterface::removeIP ( const IPAddress& p_ip ) {
    int i = hasIP( p_ip );
    int max_index = c_settings->ip_cidr_size() -1;
    if( i != -1 && i <= max_index ) { // IP is set
      std::string last_ip = c_settings->ip_cidr(max_index);
      c_settings->set_ip_cidr( i, last_ip );
      c_settings->mutable_ip_cidr()->RemoveLast();
    }
    return true;
  }

  bool NetworkInterface::addRoute ( const IPAddress& p_route, const IPAddress& p_gateway ) {
    if( p_gateway.isValid() == false ) {
      return false;
    }
    NetworkRoutePointer obj = isGatewayAlreadySet( p_gateway );
    if( obj ) {  // we already got a config object for this gateway
      return obj->addRoute( p_route );
    }
    
    NetworkRoutePointer route( new NetworkRoute() );
    if( route->setGateway( p_gateway ) && route->addRoute( p_route ) ) {
      c_routes.insert(route);
      return true;
    }
    return false;
  }

  NetworkRoutePointer NetworkInterface::getRouteByGateway ( const IPAddress& p_gateway_ip ) {
    NetworkRoutePointer r = isGatewayAlreadySet( p_gateway_ip );
    if( r ) {
      NetworkRoutePointer tmp( new tools::NetworkRoute() );
      tmp->setSettings( r->getSettings() );
      return tmp;
    }
    return r;
  }

  NetworkRoutePointer NetworkInterface::operator[] ( const IPAddress& p_gateway_ip ) {
    return getRouteByGateway( p_gateway_ip );
  }
  
  bool NetworkInterface::hasRoute ( const IPAddress& p_route ) {
    std::set<NetworkRoutePointer>::iterator rit;
    for( rit = c_routes.begin(); rit != c_routes.end(); ++rit ) {
      if( rit->get()->isRouteAlreadySet( p_route ) ) {
        return true;
      }
    }
    return false;
  }

  NetworkRoutePointer NetworkInterface::isGatewayAlreadySet ( const IPAddress& p_ip ) {
    std::set<NetworkRoutePointer>::iterator it_route;
    std::string ip = p_ip.ip();
    for( it_route = c_routes.begin(); it_route != c_routes.end(); ++it_route ) {
      if( it_route->get()->getSettings()->gateway() == ip ) {
        return *it_route;
      }
    }
    return NetworkRoutePointer();
  }
  
  int NetworkInterface::hasIP ( const IPAddress& p_ip ) const {
    int size = c_settings->ip_cidr_size();
    std::string ip = p_ip.cidr();
    for( int i = 0; i != size; ++i ) {
      if( c_settings->ip_cidr(i) == ip ) {
        return i;
      }
    }
    return -1;
  }
  
  void NetworkInterface::extractRoutesFromSettings() {
    if( c_settings->route_size() < 1 ) {
      return; // nothing to do
    }
    c_routes.clear();
    
    const pb::RepeatedPtrField<config::os::NetworkRoute>& routes = c_settings->route();
    pb::RepeatedPtrField<config::os::NetworkRoute>::const_iterator it;
    for( it = routes.begin(); it != routes.end(); ++it ) {
      NetworkRoutePointer r = isGatewayAlreadySet( it->gateway() );
      if( r == false ) {
        r = NetworkRoutePointer( new tools::NetworkRoute() );
        NetworkRouteConfigPointer c( new config::os::NetworkRoute );
        c->CopyFrom( *it );
        r->setSettings( c );
        c_routes.insert( r );
      } else {
        const pb::RepeatedPtrField<std::string>& ip = it->route_ip();
        pb::RepeatedPtrField<std::string>::const_iterator it_ip;
        for( it_ip = ip.begin(); it_ip != ip.end(); ++it_ip ) {
          r->addRoute( *it_ip );
        }
      }
    }
    // okay, all NetworkRoute objects are extracted
    // clean up
    c_settings->clear_route();
  }

}} // cloudos::tools
