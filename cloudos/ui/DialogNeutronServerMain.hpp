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



#ifndef CLOUDOS_UI_DIALOG_NEUTRON_SERVER_MAIN_HPP__
#define CLOUDOS_UI_DIALOG_NEUTRON_SERVER_MAIN_HPP__

#include <yui/YInputField.h>

#include <cloudos/tools/IPAddress.hpp>
#include <cloudos/proto/OpenStack.NeutronServer.pb.h>

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogNeutronServerMain;
  
  typedef boost::shared_ptr<DialogNeutronServerMain> DialogNeutronServerMainPointer;
  
  class DialogNeutronServerMain : public Dialog {
  public:
    DialogNeutronServerMain( short int p_dialog_flags = SHOW_BACK_BTN,
                    const std::string& p_dialog_title = "" );
    
    virtual bool setSettings( const fs::path& p_file );
    virtual bool setSettings( boost::shared_ptr<config::openstack::NeutronServer> p_settings );
    
    /**
     * Will return a pointer to an object of the copied internal storage information
     * This object is a merge of data, givven by the user, and our default values, givven
     * while constructing this object.
     * 
     * You will need to destroy the object by your own...
     */
    virtual boost::shared_ptr<config::openstack::NeutronServer> getSettings();
    
  protected:
    virtual void processUserInput();
    virtual void createDialogElements();
    
  private:
    // 
    // Our GUI elements
    // 
    YLabel      *c_lbl_intro;
    YInputField *c_input_public_ip_pool;
    
    boost::shared_ptr<config::openstack::NeutronServer> c_settings;
  };
  
}
}

#endif
