
#include <sstream>

#include "DialogInstallerFinished.hpp"

namespace cloudos {
namespace ui {
  
  DialogInstallerFinished::DialogInstallerFinished ( short int p_dialog_flags, const std::string& p_dialog_title ) : Dialog ( p_dialog_flags, p_dialog_title ) {
  }
  
  bool DialogInstallerFinished::setSettings ( const fs::path& p_file ) {
    return false; // we don't support settings
  }


  void DialogInstallerFinished::createDialogElements() {
    std::stringstream ss;
    ss << "The installation of Interactive Cloud OS is finished." << std::endl
       << "Do you wan't to reboot now? Select \"Reboot now\"" << std::endl
       << "else choose \"Abort installer\", the system will remain installed." << std::endl;
    c_lbl_intro = YUI::widgetFactory()->createLabel(c_layout_main, ss.str());
  }

  void DialogInstallerFinished::processUserInput() {

  }

  void DialogInstallerFinished::show() {
    if( c_already_shown == false ) {
      c_already_shown = true;
      
      createDialogElements();
      createBottomDialogElements();
      c_dialog->doneMultipleChanges(); // stop keep printing back and show the dialog
    }
    
    c_dialog->activate(); // make it the top most dialog and enable user input
  }

  
}
}
