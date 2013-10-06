
#ifndef CLOUDOS_UI_DIALOG_MODUESELECTION_HPP__
#define CLOUDOS_UI_DIALOG_MODUESELECTION_HPP__

#include <yui/YCheckBox.h>

#include <cloudos/proto/OS.Installer.pb.h>

#include "Dialog.hpp"

namespace cloudos {
namespace ui {
  
  class DialogModuleSelection;
  
  typedef boost::shared_ptr<DialogModuleSelection> DialogModuleSelectionPointer;
  
  class DialogModuleSelection : public Dialog {
  public:
    DialogModuleSelection( short int p_dialog_flags = SHOW_BACK_BTN,
                    const std::string& p_dialog_title = "" );
    
    virtual bool setSettings( const fs::path& p_file );
    virtual bool setSettings( boost::shared_ptr<config::os::Installer> p_settings );
    
    /**
     * Will return a pointer to an object of the copied internal storage information
     * This object is a merge of data, givven by the user, and our default values, givven
     * while constructing this object.
     * 
     * You will need to destroy the object by your own...
     */
    virtual boost::shared_ptr<config::os::Installer> getSettings();
    
  protected:
    virtual void processUserInput();
    virtual void createDialogElements();
    
  private:
    // 
    // Our GUI elements
    // 
    YLabel    *c_lbl_intro;
    YCheckBox *c_chkbox_install_management;
    YCheckBox *c_chkbox_install_keystone;
    YCheckBox *c_chkbox_install_glance;
    YCheckBox *c_chkbox_install_cinder;
    YCheckBox *c_chkbox_install_nova;
    YCheckBox *c_chkbox_install_quantum;
    
    boost::shared_ptr<config::os::Installer> c_settings;
  };
  
}
}

#endif
