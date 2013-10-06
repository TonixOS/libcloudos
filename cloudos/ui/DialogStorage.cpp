
#include <cloudos/ui/DialogStorage.hpp>

namespace cloudos {
namespace ui {
  
  DialogStorage::DialogStorage ( short int p_dialog_flags, const std::string& p_dialog_title )
                : Dialog ( p_dialog_flags, p_dialog_title ) {
    c_settings = tools::StorageLocalConfigPointer( new config::os::InstallerDisk );
    c_storage  = tools::StorageLocalPointer( new tools::StorageLocal );
  }
  
  bool DialogStorage::setSettings ( const fs::path& p_file ) {
    if( fs::exists( p_file ) == false ) {
      return false;
    }
    return tools::System::readMessage(p_file, c_settings);
  }
  
  bool DialogStorage::setSettings ( tools::StorageLocalConfigPointer p_settings ) {
    c_settings->CopyFrom( *p_settings );
    return true;
  }

  tools::StorageLocalConfigPointer DialogStorage::getSettings() {
    tools::StorageLocalConfigPointer msg( c_settings->New() );
    msg->CopyFrom( *c_settings );
    return msg;
  }

  void DialogStorage::createDialogElements() {
    std::stringstream ss;
    ss << "Please configure your local storage" << std::endl
       << "The selected disk will be ERASED and all containing data" << std::endl
       << "will be LOST!" << std::endl
       << "Please choose carefuly" << std::endl;
    c_lbl_intro = YUI::widgetFactory()->createLabel(c_layout_main, ss.str());
    
    c_cbox_disks = YUI::widgetFactory()->createComboBox(c_layout_main, "Available Local Disks");
    c_cbox_disks->setStretchable(YD_HORIZ, true);
    c_cbox_disks->setStretchable(YD_VERT,  true);
    
    std::set<tools::StorageLocalConfigPointer> disk_list = c_storage->getAvailableDisks( true );
    std::set<tools::StorageLocalConfigPointer>::const_iterator it;
    pb::uint64 biggest_hdd = 0;
    for( it = disk_list.begin(); it != disk_list.end(); ++it ) {
      std::stringstream ss;
      ss << it->get()->device_path() << " (" << it->get()->size() << "GiB) Model: "
         << it->get()->model();
      YItem *item = new YItem( ss.str() );
      
      c_device_sbox_match[item] = *it; // for later matching
      c_cbox_disks->addItem( item );
      
      // always pre-select the biggest HDD
      if( c_settings->device_path() == it->get()->device_path() || it->get()->size() > biggest_hdd ) {
        c_cbox_disks->selectItem( item );
        biggest_hdd = it->get()->size();
      }
    }
  }

  void DialogStorage::processUserInput() {
    YItem *item = c_cbox_disks->selectedItem();
    setSettings( c_device_sbox_match.at( item ) );
  }



  
}} // cloudos::ui
