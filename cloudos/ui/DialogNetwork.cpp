
#include <cloudos/ui/DialogNetwork.hpp>

namespace cloudos {
namespace ui {
  
  DialogNetwork::DialogNetwork ( short int p_dialog_flags, const std::string& p_dialog_title )
                : Dialog ( p_dialog_flags, p_dialog_title ) {
  }
  
  bool DialogNetwork::setSettings ( const fs::path& p_file ) {
    std::set<fs::path> tmp;
    tmp.insert( p_file );
    return setSettings( tmp );
  }
  
  bool DialogNetwork::setSettings ( std::set<fs::path> p_file ) {
    std::set<fs::path>::iterator i;
    std::set<tools::NetworkInterfaceConfigPointer> tmp_ifaces;
    for(i = p_file.begin(); i != p_file.end(); ++i) {
      if( fs::exists( *i ) == false ) {
        continue;
      }
      
      tools::NetworkInterfaceConfigPointer settings( new config::os::NetworkInterface );
      if( tools::System::readMessage(*i, settings) == false ) {
        continue;
      }
      //tools::NetworkInterfacePointer iface( new tools::NetworkInterface );
      //iface->setSettings(settings);
      tmp_ifaces.insert( settings );
    }
    
    if( tmp_ifaces.empty() == false ) {
      return setSettings( tmp_ifaces );
    }
    
    return false;
  }
  
  std::string DialogNetwork::getSelectedInterface() {
    return c_selected_interface;
  }

  
  bool DialogNetwork::setSettings ( std::set<tools::NetworkInterfaceConfigPointer> p_settings ) {
    std::set<tools::NetworkInterfaceConfigPointer>::iterator i;
    for( i = p_settings.begin(); i != p_settings.end(); ++i ) {
      tools::NetworkInterfacePointer iface( new tools::NetworkInterface );
      iface->setSettings( *i );
      c_system_interfaces.insert( iface );
    }
    return true;
  }

  std::set<tools::NetworkInterfaceConfigPointer> DialogNetwork::getSettings() {
    std::set<tools::NetworkInterfacePointer>::iterator i;
    std::set<tools::NetworkInterfaceConfigPointer> tmp_ifaces;
    for( i = c_system_interfaces.begin(); i != c_system_interfaces.end(); ++i ) {
      tools::NetworkInterfaceConfigPointer msg( i->get()->getSettings()->New() );
      msg->CopyFrom( *(i->get()->getSettings().get()) );
      tmp_ifaces.insert( msg );
    }
    
    return tmp_ifaces;
  }

  void DialogNetwork::createDialogElements() {
    std::stringstream ss;
    ss << "Please select your primary interface." << std::endl;
    c_lbl_intro = YUI::widgetFactory()->createLabel(c_layout_main, ss.str());
    
    const std::set<std::string> system_nics = tools::System::getAvailableInterfaces();
    
    if( system_nics.empty() ) {
      ss << "ERROR: No interface was detectable!" << std::endl
         << "Please add a network card to the system and run the installer again..." << std::endl;
      c_lbl_intro->setLabel( ss.str() );
      return;
    }
    
    c_cbox_interfaces = YUI::widgetFactory()->createComboBox(c_layout_main, "Available Interfaces");
    c_cbox_interfaces->setStretchable(YD_HORIZ, true);
    
    std::set<std::string>::const_iterator it_iface;
    for( it_iface = system_nics.begin(); it_iface != system_nics.end(); ++it_iface ) {
      c_cbox_interfaces->addItem( *it_iface );
    }
    /*
    // 
    // I P   C O N F
    // 
    YLayoutBox *layout_ip = YUI::widgetFactory()->createHBox(c_layout_main);
    c_input_ip   = YUI::widgetFactory()->createInputField(layout_ip, "");
    c_input_ip->setStretchable(YD_HORIZ, true);
    
    c_btn_add_ip = YUI::widgetFactory()->createPushButton(layout_ip, "Add IP");
    
    // 
    // R O U T E   C O N F
    // 
    YLayoutBox *layout_route = YUI::widgetFactory()->createHBox(c_layout_main);
    c_input_route     = YUI::widgetFactory()->createInputField( layout_route, "" );
    c_input_route->setStretchable(YD_HORIZ, true);
    
    YUI::widgetFactory()->createLabel(layout_route, " via ");
    
    c_input_route_via = YUI::widgetFactory()->createInputField( layout_route, "" );
    c_input_route_via->setStretchable(YD_HORIZ, true);
    
    c_btn_add_route   = YUI::widgetFactory()->createPushButton( layout_route, "Add Route" );
    
    // 
    // I P   A D D R E S S
    // 
    c_sbox_ip_addresses = YUI::widgetFactory()->createSelectionBox(c_layout_main, "Configured &IP Addresses");
    c_sbox_ip_addresses->setStretchable(YD_HORIZ, true);
    c_sbox_ip_addresses->setStretchable(YD_VERT, true);
    fillIpsSelectionBox();
    */
    // 
    // D N S
    // 
    /*std::string default_resolver_ip = "";
    if( c_settings->dns_server_size() > 0 ) {
      default_resolver_ip = c_settings->dns_server(0);
    }
    c_input_resolver_ip = createInputField("DNS Server IP: ", default_resolver_ip);*/
    /*
    // 
    // R O U T E S
    // 
    c_sbox_routes = YUI::widgetFactory()->createSelectionBox(c_layout_main, "Configured Routes");
    c_sbox_routes->setStretchable(YD_HORIZ, true);
    c_sbox_routes->setStretchable(YD_VERT, true);
    c_sbox_routes->setSendKeyEvents(true);
    c_sbox_routes->setNotify(true);
    fillRoutesSelectionBox();*/
  }
  
  void DialogNetwork::processUserEvent ( YEvent* p_event ) {
    YWidget *element = p_event->widget();
    
    /*
    if( element == c_btn_add_route ) {
      handleBtnAddRoute( p_event );
    } else if( element == c_btn_add_ip ) {
      handleBtnAddIp( p_event );
    }*/
      /*
      // validate givven IP address
      tools::IPAddress gateway_address;
      if( gateway_address.setValue( c_input_route_via->value() ) == false ) {
        c_lbl_intro->setLabel( c_lbl_intro->label() + gateway_address.error_message() );
        c_pushedBtn = DialogUserDecision::NONE;
      } else {
        c_settings->add_ip_cidr( gateway_address.cidr() );
      }
    } else if( element == c_sbox_routes ) { // handle remove route from list
      
      if( p_event->eventType() == YEvent::EventType::KeyEvent ) {
        std::string key = dynamic_cast<YKeyEvent*>(p_event)->keySymbol();
        if( key == "Del" ) {
          YItem *item = c_sbox_routes->selectedItem();
          //TODO delete the selected item...
        }
      }*/
      
    
    
  }
  
  void DialogNetwork::handleBtnAddIp ( YEvent* e ) {
    tools::IPAddress ip;
    
    if( ip.setValue( c_input_ip->value() ) == false ) {
      c_lbl_intro->setLabel( c_lbl_intro->label() + ip.error_message() );
      return;
    }
    
    std::string iface_str = c_cbox_interfaces->selectedItem()->label();
    std::map<std::string, tools::NetworkInterfacePointer>::iterator it;
    it = c_iface_name_mapping.find( iface_str );
    if( it == c_iface_name_mapping.end() ) {
      c_iface_name_mapping[iface_str] = tools::NetworkInterfacePointer( new tools::NetworkInterface );
    }
    c_iface_name_mapping[iface_str]->addIP( ip );
    fillIpsSelectionBox();
  }
  
  void DialogNetwork::handleBtnAddRoute ( YEvent* e ) {
    tools::IPAddress route_address, route_gateway;
    
    if( (route_address.setValue( c_input_route->value() ) &&
         route_gateway.setValue( c_input_route_via->value() )) == false ) { // if any of them failed
      c_lbl_intro->setLabel( c_lbl_intro->label() +
                             route_address.error_message() +
                             route_gateway.error_message() );
      return;
    }
  
    // 
    // A D D   R O U T E
    // 
    std::string iface_str = c_cbox_interfaces->selectedItem()->label();
    std::map<std::string, tools::NetworkInterfacePointer>::iterator it;
    it = c_iface_name_mapping.find( iface_str );
    if( it == c_iface_name_mapping.end() ) {
      c_iface_name_mapping[iface_str] = tools::NetworkInterfacePointer( new tools::NetworkInterface );
    }
    c_iface_name_mapping[iface_str]->addRoute( route_address, route_gateway );
    fillRoutesSelectionBox();
  }



  
  void DialogNetwork::processUserInput() {
    c_selected_interface = c_cbox_interfaces->selectedItem()->label();
    /*
    c_settings->set_name( c_sbox_ip_addresses->selectedItem()->label() );
    
    // validate givven IP address
    tools::IPAddress host_ip;
    if( host_ip.setValue( c_input_ip->value() ) == false ) {
      c_lbl_intro->setLabel( c_lbl_intro->label() + host_ip.error_message() );
      c_pushedBtn = DialogUserDecision::NONE;
    } else {
      c_settings->add_ip_cidr( host_ip.cidr() );
    }
    
    // validate givven IP address
    tools::IPAddress dns_server;
    if( dns_server.setValue( c_input_resolver_ip->value() ) == false ) {
      c_lbl_intro->setLabel( c_lbl_intro->label() + dns_server.error_message() );
      c_pushedBtn = DialogUserDecision::NONE;
    } else {
      c_settings->add_dns_server( dns_server.cidr() );
    }
    */
  }
  
  void DialogNetwork::fillIpsSelectionBox() {
    c_sbox_ip_addresses->deleteAllItems();
    c_ip_mapping.clear();
    
    std::map<std::string, tools::NetworkInterfacePointer>::iterator it_ifaces;
    for( it_ifaces = c_iface_name_mapping.begin(); it_ifaces != c_iface_name_mapping.end(); ++it_ifaces ) {
      std::string element;
      pb::RepeatedPtrField<std::string> ips = it_ifaces->second->getSettings()->ip_cidr();
      pb::RepeatedPtrField<std::string>::iterator it_ips;
      for(it_ips = ips.begin(); it_ips != ips.end(); ++it_ips) { // walk through IP addresses
        if( element.empty() ) { // first round
          element = it_ifaces->first + "\t" + *it_ips;
        } else {
          element.clear();
          element = "\t\t" + *it_ips;
        }
        YItem *item = new YItem( element );
        
        // create matching map
        c_ip_mapping[item] = std::pair<tools::NetworkInterfacePointer, std::string>( it_ifaces->second, *it_ips );
        c_sbox_ip_addresses->addItem( item );
      }
    }
  }


  void DialogNetwork::fillRoutesSelectionBox() {
    c_sbox_routes->deleteAllItems();
    c_route_mapping.clear();
    
    std::map<std::string, tools::NetworkInterfacePointer>::iterator it_ifaces;
    for( it_ifaces = c_iface_name_mapping.begin(); it_ifaces != c_iface_name_mapping.end(); ++it_ifaces ) {
      pb::RepeatedPtrField<config::os::NetworkRoute> routes = it_ifaces->second->getSettings()->route();
      pb::RepeatedPtrField<config::os::NetworkRoute>::iterator it_gateway;
      for(it_gateway = routes.begin(); it_gateway != routes.end(); ++it_gateway) { // walk through gateways
        std::string element;
        std::string route;
        for(int i = it_gateway->route_ip_size() -1; i >= 0; --i) {
          route = it_gateway->route_ip(i);
          if( element.empty() ) { // first round
            element = route + "\tvia\t" + it_gateway->gateway() + " using dev " + it_ifaces->first;
          } else {
            element.clear();
            element = route;
          }
          YItem *item = new YItem( element );
          
          tools::NetworkRouteConfigPointer r( new config::os::NetworkRoute );
          r->set_gateway( it_gateway->gateway() );
          r->add_route_ip( route );
          
          c_route_mapping[item] = std::pair<tools::NetworkInterfacePointer,
                                            tools::NetworkRouteConfigPointer >(it_ifaces->second, r);
          c_sbox_routes->addItem(item);
    } } }  // for(ifaces)->for(gateways)->for(routes)
    
  }



  
}} // cloudos::ui
