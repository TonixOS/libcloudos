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
