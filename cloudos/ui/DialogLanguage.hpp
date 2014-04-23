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



#ifndef CLOUDOS_UI_DIALOG_LANGUAGE_HPP__
#define CLOUDOS_UI_DIALOG_LANGUAGE_HPP__

#include <OS_System.pb.h>

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogLanguage;
  
  typedef boost::shared_ptr<DialogLanguage> DialogLanguagePointer;
  
  class DialogLanguage : public Dialog {
  public:
    DialogLanguage( short int p_dialog_flags = SHOW_BACK_BTN,
                    const std::string& p_dialog_title = "" );
    
    virtual bool setSettings( const fs::path& p_file );
    virtual bool setSettings( boost::shared_ptr<config::os::System> p_settings );
    
    /**
     * Will return a pointer to an object of the copied internal storage information
     * This object is a merge of data, givven by the user, and our default values, givven
     * while constructing this object.
     * 
     * You will need to destroy the object by your own...
     */
    virtual boost::shared_ptr<config::os::System> getSettings();
    
  protected:
    virtual void processUserInput();
    virtual void createDialogElements();
    
  private:
    // 
    // Our GUI elements
    // 
    YSelectionBox *c_sbox_language;
    YSelectionBox *c_sbox_keyboard;
    
    boost::shared_ptr<config::os::System> c_settings;
    
    void createLanguageSelectionBox();
    void createKeyboardSelectionBox();
  };
  
}
}

#endif
