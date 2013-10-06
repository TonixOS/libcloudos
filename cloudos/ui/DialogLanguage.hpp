
#ifndef CLOUDOS_UI_DIALOG_LANGUAGE_HPP__
#define CLOUDOS_UI_DIALOG_LANGUAGE_HPP__

#include <cloudos/proto/OS.System.pb.h>

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
