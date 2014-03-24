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



#ifndef CLOUDOS_UI_DIALOG_INSTALLER_PROCESS_HPP__
#define CLOUDOS_UI_DIALOG_INSTALLER_PROCESS_HPP__

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogInstallerProcess;
  
  typedef boost::shared_ptr<DialogInstallerProcess> DialogInstallerProcessPointer;
  
  /**
   * If you like to add a state here, please modify
   * setProcessOverview() and addState() too
   */
  enum INSTALLER_PROCESS_STATE {
    PROCESS_STATE_PREPARE_HOST_DISK     =  1,
    PROCESS_STATE_HOST_START_SETUP      =  2,
    PROCESS_STATE_MGT_VM_START_SETUP    =  3,
    PROCESS_STATE_HOST_SETUP_FINISHED   =  4,
    PROCESS_STATE_MGT_VM_SETUP_FINISHED =  5
  };
  
  class DialogInstallerProcess : public Dialog {
  public:
    DialogInstallerProcess ( short int p_dialog_flags = SHOW_BACK_BTN, const std::string& p_dialog_title = "" );
    
    virtual bool setSettings( const fs::path& p_file );
    
    virtual void show();
    
    void setNextState();
    
  protected:
    virtual void processUserInput();
    virtual void createDialogElements();
    void setProcessOverview();
    
  private:
    // 
    // Our GUI elements
    // 
    YLabel *c_lbl_intro;
    YLabel *c_lbl_states;
    YLabel *c_lbl_process_overview;
    
    unsigned short c_states_done_count;
  };
  
}
}

#endif
