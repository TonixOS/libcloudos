
#ifndef CLOUDOS_UI_DIALOG_NETWORK_HPP__
#define CLOUDOS_UI_DIALOG_NETWORK_HPP__

#include <map>

#include <yui/YComboBox.h>

#include <cloudos/core/ConfigVector.hpp>
#include <cloudos/tools/NetworkInterface.hpp>
#include <cloudos/proto/OS.Network.pb.h>

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogNetwork;
  
  typedef boost::shared_ptr<DialogNetwork> DialogNetworkPointer;
  
  /**
   * TODO: implement setConfig overrides... to ensure, that we update our iface list and configs...
   */
  class DialogNetwork : public Dialog, public core::ConfigVector<config::os::NetworkInterface> {
  public:
    DialogNetwork( short int p_dialog_flags = SHOW_BACK_BTN,
                   const std::string& p_dialog_title = "" );
    
    ConfigPointer getSelectedInterface();
    
    /**
     * Will set the selected interface
     */
    void setSelectedInterface(const std::string& p_index);
    
  protected:
    virtual void processUserEvent ( YEvent* p_event );
    virtual void processUserInput();
    virtual void createDialogElements();
    
  private:
    ConfigPointerVector::iterator c_selected_interface;
    
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
    
    /**
     * Maps YItem Pointer to NIC configs
     */
    std::map<YItem*, ConfigPointerVector::iterator> c_config_mapping;
    
    /**
     * will contain a mapping from YItem to an ip address
     * this way, we're able to lookup the ip address, which should be deleted (when requested by user)
     */
    //std::map<YItem*, std::pair<tools::NetworkInterfacePointer, std::string> > c_ip_mapping;
    /**
     * same as c_ip_mapping, just for routes
     */
    /*std::map<YItem*, std::pair<tools::NetworkInterfacePointer,
                               tools::NetworkRoute::ConfigPointer > > c_route_mapping;*/
    
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
