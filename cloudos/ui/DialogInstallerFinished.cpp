
#include <sstream>

#include "DialogInstallerFinished.hpp"

namespace cloudos {
namespace ui {
  
  DialogInstallerFinished::DialogInstallerFinished ( short int p_dialog_flags, const std::string& p_dialog_title ) : Dialog ( p_dialog_flags, p_dialog_title ) {
  }
  
  bool DialogInstallerFinished::setSettings ( const fs::path& p_file ) {
    return false; // we don't support settings
  }

  void DialogInstallerFinished::setManagementIP ( const std::string& p_mgt_ip ) {
    c_mgt_ip = p_mgt_ip;
  }

  void DialogInstallerFinished::createDialogElements() {
    std::stringstream ss;
    ss << "The installation of Interactive Cloud OS is finished." << std::endl
       << "Select \"Reboot now\" to finish the installation process," << std::endl
       << "or choose \"Abort installer\", to manually reboot later." << std::endl << std::endl
       << "After the reboot, you need to wait a few minutes," << std::endl
       << "until your management VM is running." << std::endl << " " << std::endl
       << "You will be able to reach your management interface via:" << std::endl
       << "http://" << c_mgt_ip << "/" << std::endl << " " << std::endl
       << "Use the credentials:" << std::endl
       << "\tUsername: operator" << std::endl
       << "\tPassword: cloudos" << std::endl << " " << std::endl
       << "And don't forget to eject the CD/DVD while rebooting!";
    c_lbl_intro = YUI::widgetFactory()->createLabel(c_layout_main, ss.str());
  }

  void DialogInstallerFinished::processUserInput() {

  }
  
}
}
