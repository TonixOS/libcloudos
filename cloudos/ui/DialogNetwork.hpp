
#ifndef CLOUDOS_UI_DIALOG_NETWORK_HPP__
#define CLOUDOS_UI_DIALOG_NETWORK_HPP__

#include <map>

#include <yui/YComboBox.h>

#include <cloudos/tools/NetworkInterface.hpp>

#include <cloudos/proto/OS.Network.pb.h>

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogNetwork;
  
  typedef boost::shared_ptr<DialogNetwork> DialogNetworkPointer;
  
  class DialogNetwork : public Dialog {
  public:
    DialogNetwork( short int p_dialog_flags = SHOW_BACK_BTN,
                   const std::string& p_dialog_title = "" );
    
    virtual bool setSettings( const fs::path& p_file );
    virtual bool setSettings( std::set<fs::path> p_file );
    virtual bool setSettings( std::set<tools::NetworkInterfaceConfigPointer> p_settings );
    
    /**
     * Will return a pointer to an object of the copied internal storage information
     * This object is a merge of data, givven by the user, and our default values, givven
     * while constructing this object.
     * 
     * You will need to destroy the object by your own...
     */
    virtual std::set<tools::NetworkInterfaceConfigPointer> getSettings();
    std::string getSelectedInterface(); // TODO
    
  protected:
    virtual void processUserEvent ( YEvent* p_event );
    virtual void processUserInput();
    virtual void createDialogElements();
    
  private:
    std::string c_selected_interface; // TODO
    // 
    // Our GUI elements
    // 
    YLabel        *c_lbl_intro;
    YComboBox     *c_cbox_interfaces;
    YSelectionBox *c_sbox_ip_addresses;
    YInputField   *c_input_ip;
    YInputField   *c_input_resolver_ip;
    YInputField   *c_input_route;
    YInputField   *c_input_route_via;
    YPushButton   *c_btn_add_ip;
    YPushButton   *c_btn_add_route;
    YSelectionBox *c_sbox_routes; // read only by user, will be filled by c_input_route
    
    std::map<std::string, tools::NetworkInterfacePointer> c_iface_name_mapping;
    
    /**
     * will contain a mapping from YItem to an ip address
     * this way, we're able to lookup the ip address, which should be deleted (when requested by user)
     */
    std::map<YItem*, std::pair<tools::NetworkInterfacePointer, std::string> > c_ip_mapping;
    /**
     * same as c_ip_mapping, just for routes
     */
    std::map<YItem*, std::pair<tools::NetworkInterfacePointer,
                               tools::NetworkRouteConfigPointer > > c_route_mapping;
    
    std::set<tools::NetworkInterfacePointer> c_system_interfaces;
    
    /**
     * clears c_sbox_routes and c_route_mapping
     * inserts all routes from c_system_interfaces interface configs into c_sbox_routes
     * and rebuilds the matching store c_route_mapping
     */
    void fillRoutesSelectionBox();
    
    /**
     * clears c_sbox_ip_addresses and c_ip_mapping
     * inserts all ip addresses from c_system_interfaces interface configs into c_sbox_ip_addresses
     * and rebuilds the matching store c_ip_mapping
     */
    void fillIpsSelectionBox();
    
    void handleBtnAddIp(YEvent *e);
    void handleBtnAddRoute(YEvent *e);
    
    void handleBtnRemoveIp(YEvent *e);
    void handleBtnRemoveRoute(YEvent *e);
  };
  
}
}

#endif
