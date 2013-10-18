
#include <cloudos/ui/DialogNeutronServerMain.hpp>

namespace cloudos {
namespace ui {
  
  DialogNeutronServerMain::DialogNeutronServerMain ( short int p_dialog_flags, const std::string& p_dialog_title ) 
                          : Dialog ( p_dialog_flags, p_dialog_title ) {
    c_settings = boost::shared_ptr<config::openstack::NeutronServer>( new config::openstack::NeutronServer );
  }

  bool DialogNeutronServerMain::setSettings ( const fs::path& p_file ) {
    if( fs::exists( p_file ) == false ) {
      return false;
    }
    return tools::System::readMessage(p_file, c_settings);
  }
  
  bool DialogNeutronServerMain::setSettings ( boost::shared_ptr< config::openstack::NeutronServer > p_settings ) {
    c_settings->CopyFrom( *p_settings );
    return true;
  }

  boost::shared_ptr< config::openstack::NeutronServer > DialogNeutronServerMain::getSettings() {
    boost::shared_ptr<config::openstack::NeutronServer> msg( c_settings->New() );
    msg->CopyFrom( *c_settings );
    return msg;
  }

  void DialogNeutronServerMain::createDialogElements() {
    std::stringstream intro;
    intro << std::endl << "This specifies your public / external IP range." << std::endl
          << "The first IP Address will be our default gateway." << std::endl
          << "The second will be assigned to the Interactive Cloud OS server," << std::endl
          << "the other IPs can be used for your Virtual Machines." << std::endl << std::endl
          << "Please enter your Public IP range in CIDR" << std::endl
          << "notation, e.g. 192.168.0.0/24" << std::endl << std::endl;
          
    c_lbl_intro = YUI::widgetFactory()->createLabel(c_layout_main, intro.str());
    
    c_input_public_ip_pool = YUI::widgetFactory()->createInputField(c_layout_main, "Public IP Pool (CIDR)");
    if( c_settings->has_public_ip_pool() ) {
      c_input_public_ip_pool->setValue( c_settings->public_ip_pool() );
    }
  }

  void DialogNeutronServerMain::processUserInput() {
    // validate givven IP address
    tools::IPAddress address;
    if( address.setValue( c_input_public_ip_pool->value() ) == false ) {
      c_input_public_ip_pool->setLabel( c_input_public_ip_pool->label() + address.error_message() );
      c_pushedBtn = DIALOG_DECISION_NON_BTN;
    } else {
      c_settings->set_public_ip_pool( c_input_public_ip_pool->value() );
    }
  }


  
}} // cloudos::ui
