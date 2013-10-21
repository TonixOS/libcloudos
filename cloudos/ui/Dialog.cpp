
#include "Dialog.hpp"
#include <yui/YItem.h>

namespace cloudos {
namespace ui {
  
  Dialog::Dialog ( short int p_dialog_flags, 
                   const std::string& p_dialog_title ) {
    c_already_shown = false;
    
    c_pushedBtn  = DIALOG_DECISION_NON_BTN;
    c_dialog_flags = p_dialog_flags;
    
    // create the window
    c_dialog     = YUI::widgetFactory()->createPopupDialog();
    c_layout_main = YUI::widgetFactory()->createVBox(c_dialog);
    
    // defere screen printing
    c_dialog->startMultipleChanges();
    
    // now create our title
    c_heading    = YUI::widgetFactory()->createHeading(c_layout_main, p_dialog_title);
    
    // now do your own stuff
  }
  
  void Dialog::show() {
    if( c_already_shown == false ) {
      c_already_shown = true;
      
      createDialogElements();
      createBottomDialogElements();
      c_dialog->doneMultipleChanges(); // stop keep printing back and show the dialog
    }
    
    c_dialog->activate(); // make it the top most dialog and enable user input
    waitForUserDialogDecision();
  }
  
  void Dialog::disableUserInput() {
    c_dialog->setDisabled();
  }
  
  void Dialog::setDialogTitle ( const std::string& p_dialog_title ) {
    c_heading->setText(p_dialog_title);
  }

  void Dialog::waitForUserDialogDecision() {
    // we will wait for a finishing event
    do {
      YEvent * event = c_dialog->waitForEvent();
      
      // check for our finishing buttons
      YWidget *widget = event->widget();
      if( widget == c_btn_exit ) {
        c_pushedBtn = DIALOG_DECISION_BTN_EXIT;
        return;
      }
      if( widget == c_btn_back ) {
        c_pushedBtn = DIALOG_DECISION_BTN_BACK;
      } else if( widget == c_btn_next ) {
        c_pushedBtn = DIALOG_DECISION_BTN_NEXT;
      }
      
      processUserEvent(event);
    } while( c_pushedBtn == DIALOG_DECISION_NON_BTN );
    
    processUserInput();
  }
  
  void Dialog::processUserEvent ( YEvent* p_event ) { }

  void Dialog::createBottomDialogElements() {
    // insert spacing before we create our button-frame
    YUI::widgetFactory()->createLabel(c_layout_main, " ");
    YLayoutBox *layout_finishing_btns = YUI::widgetFactory()->createHBox(c_layout_main);
    
    // the order of buttons is for usability...
    // normaly, we like to go on. So provide it as the first button...
    if( c_dialog_flags & SHOW_FINISHING_BTN ) {
      c_btn_next = YUI::widgetFactory()->createPushButton(layout_finishing_btns, "Install &NOW");
    } else {
      c_btn_next = YUI::widgetFactory()->createPushButton(layout_finishing_btns, "&Next");
    }
    YUI::widgetFactory()->createLabel(layout_finishing_btns, "  ");
    // only provide a back button, if there will be another dialog...
    if( c_dialog_flags & SHOW_BACK_BTN ) {
      c_btn_back = YUI::widgetFactory()->createPushButton(layout_finishing_btns, "&Back");
    }
    YUI::widgetFactory()->createLabel(layout_finishing_btns, "  ");
    c_btn_exit   = YUI::widgetFactory()->createPushButton(layout_finishing_btns, "&Abort Installer");
  }
  
  void Dialog::sbox_insertElementsAndSetDefault ( YSelectionBox* p_sb,
                                                const std::set<std::string>& p_elements,
                                                const std::string& p_default_element ) {
    std::set<std::string>::const_iterator i;
    for(i = p_elements.begin(); i != p_elements.end(); ++i) {
      p_sb->addItem( *i, ( *i == p_default_element ) );
    }
  }
  
  YInputField* Dialog::createInputField ( const std::string& p_label,
                                          const std::string& p_default_value,
                                          bool p_pw_field ) {
    YInputField *input = YUI::widgetFactory()->createInputField(c_layout_main, p_label, p_pw_field);
    input->setStretchable(YD_HORIZ, true);
    input->setValue(p_default_value);
    
    return input;
  }

  
  Dialog::~Dialog() {
    c_dialog->destroy();
  }
  
}
}
