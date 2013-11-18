
#include <string>
#include <sstream>

#include "DialogInstallerProcess.hpp"

namespace cloudos {
namespace ui {
  
  DialogInstallerProcess::DialogInstallerProcess ( short int p_dialog_flags, const std::string& p_dialog_title ) : Dialog ( p_dialog_flags, p_dialog_title ) {
    c_dialog_flags = c_dialog_flags | SHOW_NO_BUTTONS; // disable footer
    c_states_done_count = 0;
  }

  bool DialogInstallerProcess::setSettings ( const fs::path& p_file ) {
    return false; // we don't support settings for this screen
  }

  void DialogInstallerProcess::createDialogElements() {
    // 
    // I N T R O
    // 
    std::stringstream ss;
    ss << "Your Interactive Cloud OS is installing..." << std::endl
       << "Please wait, until all steps are finished." << std::endl
       << "This might take a few minutes.";
    c_lbl_intro = YUI::widgetFactory()->createLabel(c_layout_main, ss.str());
    YUI::widgetFactory()->createLabel(c_layout_main, "");
    c_lbl_process_overview = YUI::widgetFactory()->createLabel(c_layout_main, "");
    c_lbl_states = YUI::widgetFactory()->createLabel(c_layout_main, "");
  }

  void DialogInstallerProcess::setProcessOverview() {
    unsigned short state_max_count = 5;
    
    std::stringstream ss;
    ss << c_states_done_count << " of " << state_max_count << " finished" << std::endl << std::endl;
    c_layout_main->removeChild( c_lbl_process_overview );
    c_lbl_process_overview = YUI::widgetFactory()->createLabel(c_layout_main, ss.str());
  }

  void DialogInstallerProcess::setNextState() {
    std::stringstream ss;
    ss << c_lbl_states->text();
    switch( ++c_states_done_count ) {
      case PROCESS_STATE_PREPARE_HOST_DISK:
        ss << "Preparing host disk..." << std::endl;
        break;
      case PROCESS_STATE_HOST_START_SETUP:
        ss << "Starting host setup..." << std::endl;
        break;
      case PROCESS_STATE_MGT_VM_START_SETUP:
        ss << "Starting management VM setup..." << std::endl;
        break;
      case PROCESS_STATE_HOST_SETUP_FINISHED:
        ss << "Finished host setup!" << std::endl;
        break;
      case PROCESS_STATE_MGT_VM_SETUP_FINISHED:
        ss << "Finished management VM setup!" << std::endl;
        break;
      default:
        break; //FIXME: report an error
    }
    c_layout_main->removeChild(c_lbl_states);
    
    setProcessOverview();
    c_lbl_states = YUI::widgetFactory()->createLabel(c_layout_main, ss.str());
    c_dialog->recalcLayout();
    c_dialog->waitForEvent(1);
  }

  void DialogInstallerProcess::processUserInput() {

  }

  void DialogInstallerProcess::show() {
    if( c_already_shown == false ) {
      c_already_shown = true;
      
      createDialogElements();
      createBottomDialogElements();
      c_dialog->doneMultipleChanges(); // stop keep printing back and show the dialog
    }

    c_dialog->activate(); // make it the top most dialog and enable user input
    c_dialog->open();
  }

  
}
}
