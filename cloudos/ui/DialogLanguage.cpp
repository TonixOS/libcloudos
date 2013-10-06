
#include "DialogLanguage.hpp"

namespace cloudos {
namespace ui {

  DialogLanguage::DialogLanguage ( short int p_dialog_flags,
                                   const std::string& p_dialog_title )
                 : Dialog ( p_dialog_flags, p_dialog_title ) {
    
     c_settings = boost::shared_ptr<config::os::System>( new config::os::System );
  }
  
  bool DialogLanguage::setSettings ( const fs::path& p_file ) {
    if( fs::exists( p_file ) == false ) {
      return false;
    }
    return tools::System::readMessage(p_file, c_settings);
  }
  
  bool DialogLanguage::setSettings ( boost::shared_ptr< config::os::System > p_settings ) {
    c_settings->CopyFrom( *p_settings );
    return true;
  }

  
  boost::shared_ptr<config::os::System> DialogLanguage::getSettings() {
    boost::shared_ptr<config::os::System> msg( c_settings->New() );
    msg->CopyFrom( *c_settings );
    return msg;
  }


  void DialogLanguage::createDialogElements() {
    createLanguageSelectionBox();
    createKeyboardSelectionBox();
  }
  
  void DialogLanguage::createLanguageSelectionBox() {
    c_sbox_language = YUI::widgetFactory()->createSelectionBox(c_layout_main, "Select System Language");
    c_sbox_language->setStretchable(YD_HORIZ, true);
    
    std::set<std::string> languages;
    languages.insert( "en_GB.uft8" );
    languages.insert( "de_DE.utf8" );
    
    sbox_insertElementsAndSetDefault(c_sbox_language, languages, c_settings->locale_charset());
  }
  
  void DialogLanguage::createKeyboardSelectionBox() {
    c_sbox_keyboard = YUI::widgetFactory()->createSelectionBox(c_layout_main, "Select Keyboard Layout");
    c_sbox_keyboard->setStretchable(YD_HORIZ, true);
    
    std::set<std::string> keyboardLayouts;
    keyboardLayouts.insert( "en" );
    keyboardLayouts.insert( "de" );
    
    sbox_insertElementsAndSetDefault(c_sbox_keyboard, keyboardLayouts, c_settings->keyboard_layout());
  }
  
  void DialogLanguage::processUserInput() {
    // get selected language
    c_settings->set_locale_charset( c_sbox_language->selectedItem()->label() );
    
    // get selected keyboard layout
    c_settings->set_keyboard_layout( c_sbox_keyboard->selectedItem()->label() );
  }

}
}