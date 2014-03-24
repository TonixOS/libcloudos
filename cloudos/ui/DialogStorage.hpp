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



#ifndef CLOUDOS_UI_DIALOG_STORAGE_HPP__
#define CLOUDOS_UI_DIALOG_STORAGE_HPP__

#include <yui/YComboBox.h>

#include <cloudos/core/ConfigVector.hpp>
#include <cloudos/system/HDDisk.hpp>

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogStorage;
  
  typedef boost::shared_ptr<DialogStorage> DialogStoragePointer;
  
  class DialogStorage : public Dialog, public core::ConfigVector<config::os::hw::HDDisk> {
  public:
    DialogStorage( short int p_dialog_flags = SHOW_BACK_BTN,
                   const std::string& p_dialog_title = "" );
    
    /**
     * Will return the disk config object, of the selected disk
     */
    virtual ConfigPointer getSelectedDisk();
    
    /**
     * Sets the selected disk.
     * 
     * index is the config::os::hw::HDDisk->index aka device_path
     */
    virtual void setSelectedDisk(const std::string& p_index);
    
  protected:
    virtual void processUserInput();
    virtual void createDialogElements();
    
  private:
    // 
    // Our GUI elements
    // 
    YLabel    *c_lbl_intro;
    YComboBox *c_cbox_disks;
    
    /**
     * Maps YItem Pointer to HDDisk path, so we could find the corresponding config later on
     */
    std::map<YItem*, ConfigPointerVector::iterator> c_config_mapping;
    
    /**
     * Our selected disk
     */
    ConfigPointerVector::iterator c_selected_disk;
  };
  
}
}

#endif
