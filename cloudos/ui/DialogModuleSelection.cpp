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



#include "DialogModuleSelection.hpp"

namespace cloudos {
namespace ui {
  
  DialogModuleSelection::DialogModuleSelection ( short int p_dialog_flags, const std::string& p_dialog_title ) 
                        : Dialog ( p_dialog_flags, p_dialog_title ) {
    c_settings = boost::shared_ptr<config::os::Installer>( new config::os::Installer );
  }
  
  bool DialogModuleSelection::setSettings ( const fs::path& p_file ) {
    if( fs::exists( p_file ) == false ) {
      return false;
    }
    return tools::System::readMessage(p_file, c_settings);
  }

  bool DialogModuleSelection::setSettings ( boost::shared_ptr<config::os::Installer> p_settings ) {
    c_settings->CopyFrom( *p_settings );
    return true;
  }

  boost::shared_ptr< config::os::Installer > DialogModuleSelection::getSettings() {
    boost::shared_ptr<config::os::Installer> msg( c_settings->New() );
    msg->CopyFrom( *c_settings );
    return msg;
  }

  
  void DialogModuleSelection::createDialogElements() {
    std::stringstream intro;
    intro << "Please select all components, which should be installed." << std::endl
          << "In doubt, keep settings untouched"                        << std::endl;
    c_lbl_intro = YUI::widgetFactory()->createLabel( c_layout_main, intro.str() );
    
    // 
    // checkboxes
    // 
    c_chkbox_install_management = YUI::widgetFactory()->createCheckBox( c_layout_main, "Management System",
                                                                        c_settings->install_management() );
    c_chkbox_install_keystone   = YUI::widgetFactory()->createCheckBox( c_layout_main, "Identity (Keystone)",
                                                                        c_settings->install_keystone() );
    c_chkbox_install_glance     = YUI::widgetFactory()->createCheckBox( c_layout_main, "Image (Glance)",
                                                                        c_settings->install_glance() );
    c_chkbox_install_cinder     = YUI::widgetFactory()->createCheckBox( c_layout_main, "Volume (Cinder)",
                                                                        c_settings->install_cinder() );
    c_chkbox_install_nova       = YUI::widgetFactory()->createCheckBox( c_layout_main, "Compute (Nova)",
                                                                        c_settings->install_nova() );
    c_chkbox_install_quantum    = YUI::widgetFactory()->createCheckBox( c_layout_main, "Network (Neutron)",
                                                                        c_settings->install_quantum() );
  }

  void DialogModuleSelection::processUserInput() {
    c_settings->set_install_management( c_chkbox_install_management->isChecked() );
    c_settings->set_install_keystone(   c_chkbox_install_keystone->isChecked()   );
    c_settings->set_install_glance(     c_chkbox_install_glance->isChecked()     );
    c_settings->set_install_cinder(     c_chkbox_install_cinder->isChecked()     );
    c_settings->set_install_nova(       c_chkbox_install_nova->isChecked()       );
    c_settings->set_install_quantum(    c_chkbox_install_quantum->isChecked()    );
    
    //FIXME: Workaround, let the user choose
    if( c_settings->install_keystone() ) {
      c_settings->set_install_mariadb(  true );
      c_settings->set_install_rabbitmq( true );
    } else {
      c_settings->set_install_mariadb(  false );
      c_settings->set_install_rabbitmq( false );
    }
  }
}
}