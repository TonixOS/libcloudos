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



#ifndef CLOUDOS_UI_DIALOG_INSTALLER_FINISHED_HPP__
#define CLOUDOS_UI_DIALOG_INSTALLER_FINISHED_HPP__

#include <string>

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogInstallerFinished;
  
  typedef boost::shared_ptr<DialogInstallerFinished> DialogInstallerFinishedPointer;
  
  class DialogInstallerFinished : public Dialog {
  public:
    DialogInstallerFinished ( short int p_dialog_flags = SHOW_BACK_BTN, const std::string& p_dialog_title = "" );
    
    virtual bool setSettings( const fs::path& p_file );
    
    void setManagementIP( const std::string& p_mgt_ip );
    
  protected:
    virtual void processUserInput();
    virtual void createDialogElements();
    
  private:
    // 
    // Our GUI elements
    // 
    YLabel *c_lbl_intro;
    
    std::string c_mgt_ip;
  };
  
}
}

#endif
