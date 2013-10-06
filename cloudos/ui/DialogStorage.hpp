
#ifndef CLOUDOS_UI_DIALOG_STORAGE_HPP__
#define CLOUDOS_UI_DIALOG_STORAGE_HPP__

#include <yui/YComboBox.h>

#include <cloudos/tools/StorageLocal.hpp>

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogStorage;
  
  typedef boost::shared_ptr<DialogStorage> DialogStoragePointer;
  
  class DialogStorage : public Dialog {
  public:
    DialogStorage( short int p_dialog_flags = SHOW_BACK_BTN,
                   const std::string& p_dialog_title = "" );
    
    virtual bool setSettings( const fs::path& p_file );
    virtual bool setSettings( tools::StorageLocalConfigPointer p_settings );
    
    /**
     * Will return a pointer to an object of the copied internal storage information
     * This object is a merge of data, givven by the user, and our default values, givven
     * while constructing this object.
     * 
     * You will need to destroy the object by your own...
     */
    virtual tools::StorageLocalConfigPointer getSettings();
    
  protected:
    virtual void processUserInput();
    virtual void createDialogElements();
    
  private:
    // 
    // Our GUI elements
    // 
    YLabel    *c_lbl_intro;
    YComboBox *c_cbox_disks;
    
    std::map<YItem*, tools::StorageLocalConfigPointer> c_device_sbox_match;
    
    tools::StorageLocalConfigPointer c_settings;
    tools::StorageLocalPointer c_storage;
  };
  
}
}

#endif
