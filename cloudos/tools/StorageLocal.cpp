
#include <cloudos/tools/StorageLocal.hpp>

namespace cloudos {
namespace tools {
  
  StorageLocal::StorageLocal() {
    c_settings = StorageLocalConfigPointer( new config::os::InstallerDisk );
  }
  
  StorageLocal::~StorageLocal() {
    if( c_available_disks.empty() == false ) {
      c_available_disks.clear();
      ped_device_free_all();
    }
  }

  bool StorageLocal::setSettings ( const fs::path& p_file ) {
    if( fs::exists( p_file ) == false ) {
      return false;
    }
    return tools::System::readMessage(p_file, c_settings);
  }

  bool StorageLocal::setSettings ( StorageLocalConfigPointer p_settings ) {
    c_settings->CopyFrom( *p_settings );
    return true;
  }

  StorageLocalConfigPointer StorageLocal::getSettings() {
    StorageLocalConfigPointer msg( c_settings->New() );
    msg->CopyFrom( *c_settings );
    return msg;
  }

  const std::set<StorageLocalConfigPointer> StorageLocal::getAvailableDisks() {
    if( c_available_disks.empty() ) {
      // scan for disks
      ped_device_probe_all();
      // 0 will indicate, that we need the first element, instead of "next"
      PedDevice *device;
      for( device = ped_device_get_next(0); device; device = ped_device_get_next(device) ) {
        c_available_disks[ device->path ] = device;
      }
    }
    
    std::set<StorageLocalConfigPointer> list;
    std::map<std::string, PedDevice*>::iterator i;
    for(i = c_available_disks.begin(); i != c_available_disks.end(); ++i) {
      StorageLocalConfigPointer storage( new config::os::InstallerDisk );
      storage->set_device_path( i->first );
      storage->set_model( i->second->model );
      storage->set_size( calculateSize( i->second->hw_geom , i->second->sector_size ) );
      list.insert( storage );
    }
    return list;
  }
  
  bool StorageLocal::applyToSystem() {
    PedDevice *device = c_available_disks[c_settings->device_path()];
    
    PedDiskType *type;
    if( c_settings->size() > 2000 ) { // gt 2TB
      type = ped_disk_type_get("gpt");
    } else {
      type = ped_disk_type_get("msdos");
    }
    
    if( type == nullptr ) {
      std::cerr << "Error while getting partition type for " << c_settings->device_path() << std::endl;
      return false;
    }
    
    PedDisk *disk = ped_disk_new_fresh(device, type);
    if( disk == nullptr ) {
      std::cerr << "Error while creating a new partition for " << c_settings->device_path() << std::endl;
      return false;
    }
    
    // create new partition
    PedFileSystemType *fs_type = ped_file_system_type_get("ext2");
    if( fs_type == nullptr ) {
      std::cerr << "Error while getting fs type ext4 FILE/LINE" << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    PedSector max_sector = ped_disk_max_partition_length( disk );
    PedSector start_sector = 2048;
    
    PedSector first_part_end = getSectorCounterForNumber(100, 'M') + start_sector;
    if( first_part_end == 0 ) {
      std::cerr << "Error while calculating first_part_end " << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    if( first_part_end > max_sector ) {
      std::cerr << "Error first_part_end exeeded max_sector: " << first_part_end << "tried and max: " << max_sector << "max " << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    PedPartition *part = ped_partition_new(disk, PED_PARTITION_NORMAL, fs_type, start_sector, first_part_end);
    
    if( part == nullptr ) {
      std::cerr << "Error while creating first partition" << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    PedConstraint *constraint = ped_device_get_constraint(device);
    
    if( ped_disk_add_partition(disk, part, constraint) == 0 ) {
      std::cerr << "Error while adding first partition" << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    if( ped_disk_commit_to_dev(disk) == 0 ) {
      std::cerr << "Error while commit first partition to dev" << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    if( ped_disk_commit_to_os(disk) == 0 ) {
      std::cerr << "Error while commit first partition to os" << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    return false;
  }
  
  PedSector StorageLocal::getSectorCounterForNumber ( unsigned int p_value, char p_unit ) {
    unsigned long long return_value = p_value;
    
    short iterator = 0;
    switch( p_unit ) {
      case 'E':
        ++iterator;
      case 'T':
        ++iterator;
      case 'G':
        ++iterator;
      case 'M':
        ++iterator;
      case 'K':
        ++iterator;
        break;
      default:
        return 0;
    }
    for( ; iterator != 0; --iterator  ) {
      return_value = return_value * 1024;
    }
    return_value = return_value * 8; // convert to bytes
    
    return (PedSector) return_value / c_available_disks[ c_settings->device_path() ]->sector_size;
  }



  unsigned long long StorageLocal::calculateSize ( PedCHSGeometry p_geometry, long p_sector_size ) {
    unsigned long long size = p_geometry.cylinders * p_geometry.heads * p_geometry.sectors * p_sector_size;
    
    // now calculate down to GB        KiB    MiB    GiB
    size = (unsigned long long) size / 1024 / 1024 / 1024;
    return size;
  }



  
}} // cloudos::tools
