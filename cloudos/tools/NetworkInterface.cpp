
#include <cloudos/tools/NetworkInterface.hpp>

namespace cloudos {
namespace tools {
  
  NetworkInterface::NetworkInterface() {
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
  
  bool NetworkInterface::removeIP ( NetworkInterfaceConfig* p_config, IPAddress && p_ip ) {
    Logger::LogSourceType lg;
    
    if( p_config == nullptr ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "p_config is nullptr... Abort!";
      return false;
    }
    
    if( p_ip.isValid() == false ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "ip is invalid... Abort!";
      return false;
    }
    
    const std::string& ip = p_ip.cidr();
    int index = 0;
    int max_index = p_config->ip_cidr_size() -1;
    
    while( index <= max_index && ip != p_config->ip_cidr(index) ) {
      ++index;
    }
    
    if( index > max_index ) {
      BOOST_LOG_SEV(lg, blog::trivial::debug) << "ip not found " << p_ip.cidr();
      return true;
    }
    
    if( index == max_index ) { // swap element position to last element...
      p_config->set_ip_cidr( index, p_config->ip_cidr(max_index) );
    }
    
    BOOST_LOG_SEV(lg, blog::trivial::debug) << "removing ip " << p_ip.cidr();
    p_config->mutable_ip_cidr()->RemoveLast();
    
    return true;
  }
  
  bool NetworkInterface::addIP ( NetworkInterfaceConfig* p_config, IPAddress && p_ip ) {
    Logger::LogSourceType lg;
    
    if( p_config == nullptr ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "p_config is nullptr... Abort!";
      return false;
    }
    
    if( p_ip.isValid() == false ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "ip is invalid (" << p_ip.error_message() << ")... Abort!";
      return false;
    }
    
    if( NetworkInterface::ipExists( p_config, p_ip ) ) {
      BOOST_LOG_SEV(lg, blog::trivial::debug) << "ip address already exist " << p_ip.cidr();
      return true;
    }
    
    BOOST_LOG_SEV(lg, blog::trivial::debug) << "adding ip address " << p_ip.cidr();
    p_config->add_ip_cidr( p_ip.cidr() );
    return true;
  }
  
  bool NetworkInterface::ipExists ( NetworkInterfaceConfig* p_config, const IPAddress& p_ip ) {
    Logger::LogSourceType lg;
    
    if( p_config == nullptr ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "p_config is nullptr... Abort!";
      return false;
    }
    
    if( p_ip.isValid() == false ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "ip is invalid... Abort!";
      return false;
    }
    
    const std::string& ip = p_ip.cidr();
    for( auto& ip2 : p_config->ip_cidr() ) {
      if( ip == ip2 ) {
        BOOST_LOG_SEV(lg, blog::trivial::debug) << "ip exists";
        return true;
      }
    }
    BOOST_LOG_SEV(lg, blog::trivial::debug) << "ip does not exist";
    return false;
  }

  bool NetworkInterface::addRoute( NetworkInterfaceConfig* p_config, IPAddress&& p_route, const IPAddress& p_gateway ) {
    return NetworkRoute::addRoute( getRouteConfigObject(p_config, p_gateway), std::move(p_route) );
  }
  
  NetworkRouteConfig* NetworkInterface::getRouteConfigObject( NetworkInterfaceConfig* p_config, const IPAddress& p_gateway ) {
    Logger::LogSourceType lg;
    
    if( p_config == nullptr ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "p_config is nullptr... Abort!";
      return nullptr;
    }
    
    if( p_gateway.isValid() == false ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "p_gateway is invalid, could not check for a config object... Abort!";
      return nullptr;
    }
    
    NetworkRouteConfig* route = NetworkInterface::gatewayExists( p_config, p_gateway );
    
    if( route != nullptr ) {
      return route;
    }
    
    BOOST_LOG_SEV(lg, blog::trivial::debug) << "creating new route config object";
    route = p_config->add_route();
    route->set_index( p_gateway.ip() );
    return route;
  }
  
  NetworkRouteConfig* NetworkInterface::gatewayExists ( NetworkInterfaceConfig* p_config, const IPAddress& p_gateway ) {
    Logger::LogSourceType lg;
    BOOST_LOG_SEV(lg, blog::trivial::debug) << "check if gateway exists...";
    
    if( p_config == nullptr ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "p_config is nullptr... Abort!";
      return nullptr;
    }
    
    if( p_gateway.isValid() == false ) {
      BOOST_LOG_SEV(lg, blog::trivial::error) << "p_gateway is invalid... Abort!";
    }
    
    for(int pos = 0, pos_max = p_config->route_size(); pos < pos_max; ++pos ) {
      if( p_config->route(pos).index() == p_gateway.ip() ) {
        BOOST_LOG_SEV(lg, blog::trivial::debug) << "found route by gateway";
        return p_config->mutable_route(pos);
      }
    }
    BOOST_LOG_SEV(lg, blog::trivial::debug) << "gateway does not exist jet";
    return nullptr;
  }
}} // cloudos::tools
