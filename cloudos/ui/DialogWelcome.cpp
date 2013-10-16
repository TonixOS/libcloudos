
#include "DialogWelcome.hpp"

namespace cloudos {
namespace ui {
  
  DialogWelcome::DialogWelcome ( short int p_dialog_flags,
                                 const std::string& p_dialog_title )
                        : Dialog ( p_dialog_flags, p_dialog_title ) {
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
    if( c_welcome_text.rdbuf()->in_avail() == 0 ) {
      c_welcome_text << "Welcome to Interactive Cloud OS (Version __CLOUDOS__VERSION__)" << std::endl
                     << "Warning: THIS INSTALLATION WILL ERASE ANY EXISTING DATA ON YOUR DISKS."  << std::endl
                     << "DO YOU WANT TO PROCEED?" << std::endl;
    }
    c_lb_welcome_text = YUI::widgetFactory()->createLabel( c_layout_main, c_welcome_text.str() );
  }

  void DialogWelcome::processUserInput() {
    // we won't do anything here, as we're just showing some text...
  }
  
}
}
