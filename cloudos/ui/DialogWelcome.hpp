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



#ifndef CLOUDOS_UI_DIALOG_WELCOME_HPP__
#define CLOUDOS_UI_DIALOG_WELCOME_HPP__

#include <fstream>

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogWelcome;
  
  typedef boost::shared_ptr<DialogWelcome> DialogWelcomePointer;
  
  enum SystemsRequirementsMeet {
    SYSTEMREQUIREMENTS_MEMORY = 1,
    SYSTEMREQUIREMENTS        = 1 // all requirements included
  };
  
  class DialogWelcome : public Dialog {
  public:
    DialogWelcome( short int p_dialog_flags = SHOW_BACK_BTN,
                   const std::string& p_dialog_title = "" );
    
    /**
     * Will read the file and display it's content as welcome_text in the dialog.
     * If the givven file is empty, we will ignore it and return false.
     */
    virtual bool setSettings( const fs::path& p_file );
    
  protected:
    virtual void processUserInput();
    virtual void createDialogElements();
    
  private:
    std::stringstream c_welcome_text;
    YLabel *c_lb_welcome_text;
    
    uint c_system_requirements_meet;
  };
  
}
}

#endif
