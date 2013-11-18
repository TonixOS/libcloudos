
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
    
    virtual void show();
    
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
