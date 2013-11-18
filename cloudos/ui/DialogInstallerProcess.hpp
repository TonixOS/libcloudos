
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
