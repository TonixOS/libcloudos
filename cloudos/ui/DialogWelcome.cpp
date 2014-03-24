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



extern "C" {
#include <sys/sysinfo.h>
}

#include "DialogWelcome.hpp"

namespace cloudos {
namespace ui {
  
  DialogWelcome::DialogWelcome ( short int p_dialog_flags,
                                 const std::string& p_dialog_title )
                        : Dialog ( p_dialog_flags, p_dialog_title ) {
    
    boost::shared_ptr<struct sysinfo> system(new struct sysinfo);
    if( sysinfo(system.get()) == 0 ) {
      const long required_memory = 6000000000; // Bytes (6GB) => 6000 * 1000 * 1000
      if( system->totalram > required_memory ) {
        c_system_requirements_meet = c_system_requirements_meet | SYSTEMREQUIREMENTS_MEMORY;
      }
    } else {
      std::cerr << "faild calling sysinfo() within " << __FILE__ <<  __FUNCTION__ << std::endl;
      c_system_requirements_meet = false;
    }
    
  }
  
  bool DialogWelcome::setSettings ( const fs::path& p_file ) {
    // we're unable to load any configuration file here...
    // as we just display some text
    // that said, we're expecting a text file as "configuration" file
    
    if( fs::exists( p_file ) == false ) {
      return false;
    }
    if( fs::is_empty( p_file ) == true ) {
      return false; //won't read an empty file, in this case, just display our default text
    }
    
    // reset text
    c_welcome_text.clear();
    
    // all content of this file will be displayed
    tools::FStreamPointer inputFile( new std::fstream );
    inputFile->open( p_file.string().c_str(), std::ios::in );
    std::string line;
    while(inputFile->good()) {
      std::getline( *(inputFile.get()), line);
      c_welcome_text << line << std::endl;
      line.clear();
    }
    return true;
  }

  
  void DialogWelcome::createDialogElements() {
    // if there are some requirements not meet,
    // we will get a result greater zero
    if( c_system_requirements_meet != SYSTEMREQUIREMENTS ) {
      c_welcome_text << "You don't seem to have enought RAM installed." << std::endl
                     << "Interactive Cloud OS needs at least 6GB of RAM";
    } else if( c_welcome_text.rdbuf()->in_avail() == 0 ) { // if c_welcome_text is empty
      c_welcome_text << "Welcome to Interactive Cloud OS (Version __CLOUDOS__VERSION__)" << std::endl
                     << "Warning: THIS INSTALLATION WILL ERASE ANY EXISTING DATA ON YOUR DISKS."  << std::endl
                     << "DO YOU WANT TO PROCEED?" << std::endl;
    }
    c_lb_welcome_text = YUI::widgetFactory()->createLabel( c_layout_main, c_welcome_text.str() );
  }

  void DialogWelcome::processUserInput() {
    // we won't do anything here, as we're just showing some text...
  }
  
}}
