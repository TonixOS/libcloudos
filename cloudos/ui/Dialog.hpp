
#ifndef CLOUDOS_UI_DIALOG_HPP__
#define CLOUDOS_UI_DIALOG_HPP__

#include <set>
#include <string>
#include <sstream>

#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>

#include <google/protobuf/message.h>

#include <yui/YUI.h>
#include <yui/YEvent.h>
#include <yui/YWidgetFactory.h>
#include <yui/YDialog.h>
#include <yui/YInputField.h>
#include <yui/YItem.h>
#include <yui/YLabel.h>
#include <yui/YLayoutBox.h>
#include <yui/YProperty.h>
#include <yui/YPushButton.h>
#include <yui/YSelectionBox.h>

#include <cloudos/tools/System.hpp>

namespace fs = boost::filesystem;
namespace pb = google::protobuf;

namespace cloudos {
namespace ui {
  
  class Dialog;
  
  typedef boost::shared_ptr<Dialog> DialogPointer;
  typedef boost::shared_ptr<pb::Message> MessagePointer;
  
  enum DialogUserDecision {
    DIALOG_DECISION_NON_BTN    = 0, // default value
    DIALOG_DECISION_BTN_BACK   = 1,
    DIALOG_DECISION_BTN_EXIT   = 2,
    DIALOG_DECISION_BTN_NEXT   = 3
  };
  
  enum DialogEnvironmentFlags {
    NO_FLAG            =  0,
    SHOW_BACK_BTN      =  1,
    SHOW_FINISHING_BTN =  2,
    SHOW_REBOOT_BTN    =  4,
    SHOW_NO_BUTTONS    = 16
  };
  
  /**
   * Is an abstract class for an ncurses configuration dialog.
   */
  class Dialog {
  public:
    /**
     * Will change the current dialog title
     */
    void setDialogTitle( const std::string& p_dialog_title );
    
    /**
     * If the dialog is shown the first time.
     * it will be filled with items and then shown
     * 
     * If the dialog is shown otherwise,
     * it will be pushed to the front of the printing-stack
     */
    virtual void show();
    
    /**
     * Will disable the ability for the user, to change the data
     * show() will enable the ability automaticly again!
     * 
     * Call this function, if you like to prepare the dialog to be hidden
     */
    void disableUserInput();
    
    /**
     * Will wait until the user presses one of the finishing buttons
     * like Exit, Back or Next
     */
    virtual void waitForUserDialogDecision();
    
    /**
     * Which buttons was pushed by the user?
     */
    inline DialogUserDecision getPushedBtn() { return c_pushedBtn; }
    
    void reportErrorToUser(std::string&& p_msg) const;
    
    ~Dialog();
    
  protected:
    MessagePointer c_settings;
    
    DialogUserDecision c_pushedBtn;
    
    YDialog *c_dialog;
    YLayoutBox *c_layout_main;
    YLabel *c_heading;
    
    YPushButton *c_btn_back;
    YPushButton *c_btn_exit;
    YPushButton *c_btn_next;
    
    short int c_dialog_flags;
    
    bool c_already_shown;
    
    /**
     * Will create the dialog box
     */
    Dialog( short int p_dialog_flags = SHOW_BACK_BTN,
            const std::string& p_dialog_title = "" );
    
    /**
     * process the triggered event (like adding a value in field X)
     */
    virtual void processUserEvent(YEvent *p_event);
    
    /**
     * After the user pushed a finishing button,
     * this function is called to validate/store the givven input
     * 
     * if you need the user to correct his input, just reset the c_pushedBtn to DialogUserDecision::NONE
     * and inform the user...
     */
    virtual void processUserInput() = 0;
    
    /**
     * Creates all YItems of the dialog
     * except the finishing buttons, which will be created by ourself
     */
    virtual void createDialogElements() = 0;
    
    /**
     * Will create the bottom line of the dialog
     * with all finishing buttons
     */
    void createBottomDialogElements();
    
    /**
     * Inserts elements into the YSelectionBox
     * and will pre-select the givven default element
     */
    void sbox_insertElementsAndSetDefault(YSelectionBox* p_sb,
                                     const std::set<std::string>& p_elements,
                                     const std::string& p_default_element);
    
    YInputField* createInputField(const std::string& p_label, const std::string& p_default_value, bool p_pw_field = false);
  };
  
}
}

#endif
