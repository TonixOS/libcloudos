
#include <cloudos/ui/DialogStorage.hpp>
#include <cloudos/tools/System.hpp>

namespace cloudos {
namespace ui {
  
  DialogStorage::DialogStorage ( short int p_dialog_flags, const std::string& p_dialog_title )
                : Dialog ( p_dialog_flags, p_dialog_title ) {
    c_selected_disk = c_configs.end();
  }

  void DialogStorage::createDialogElements() {
    std::stringstream ss;
    ss << "Please select your storage area where Interactive Cloud OS should be installed." << std::endl << std::endl
       << "WARNING: The selected area will be erased and all data will be lost! If you don’t want to proceed- STOP NOW!" << std::endl
       << std::endl << std::endl;
    c_lbl_intro = YUI::widgetFactory()->createLabel(c_layout_main, ss.str());
    
    c_cbox_disks = YUI::widgetFactory()->createComboBox(c_layout_main, "Please select your installation disk space:");
    c_cbox_disks->setStretchable(YD_HORIZ, true);
    c_cbox_disks->setStretchable(YD_VERT,  true);
    
    c_configs = std::move( tools::System::getAvailableHDDisks(true) );
    
    if( c_configs.empty() ) {
      LOG_E() << "no disk configs available... Abort!";
      return;
    }
    
    // pre-select biggest disk
    pb::uint64 biggest_size = 0;
    for(auto disk = c_configs.begin(), end = c_configs.end(); disk != end; ++disk ) {
      std::stringstream tmpss;
      tmpss << (*disk)->index() << " (" << (*disk)->size() << "GiB) Model: " << (*disk)->model();
      YItem* item = new YItem( tmpss.str() );
      
      c_config_mapping[item] = disk;
      
      c_cbox_disks->addItem(item);
      if( biggest_size < (*disk)->size() ) {
        c_cbox_disks->selectItem(item);
        biggest_size = (*disk)->size();
      }
    }
    
    // override pre-selection to c_selected_disk, if c_selected_disk is set
    if( c_selected_disk != c_configs.end() ) {
      for(auto pos : c_config_mapping) {
        if( pos.second == c_selected_disk ) {
          c_cbox_disks->selectItem(pos.first);
          break;
        }
      }
    }
    LOG_D() << "pre-selected disk: " << c_config_mapping[ c_cbox_disks->selectedItem() ]->get()->index();
  }
  
  DialogStorage::ConfigPointer DialogStorage::getSelectedDisk() {
    if( c_selected_disk == c_configs.end() ) {
      return ConfigPointer( new config::os::hw::HDDisk() );
    }
    
    ConfigPointer copy( new config::os::hw::HDDisk() );
    copy->CopyFrom( *(c_selected_disk->get()) );
    return copy;
  }
  
  void DialogStorage::setSelectedDisk(const std::string& p_index) {
    c_selected_disk = findConfigByIndex( p_index );
  }

  void DialogStorage::processUserInput() {
    std::string index( c_config_mapping[c_cbox_disks->selectedItem()]->get()->index() );
    LOG_I() << "user selected disk: " << index;
    c_selected_disk = findConfigByIndex( std::move(index) );
  }



  
}} // cloudos::ui
