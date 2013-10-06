
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
    if( c_disk == nullptr ) {
      return false;
    }
    
    if( ped_disk_commit_to_dev(c_disk) == 0 ) {
      std::cerr << "Error while commit first partition to dev" << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    if( ped_disk_commit_to_os(c_disk) == 0 ) {
      std::cerr << "Error while commit first partition to os" << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    return true;
  }
  
  bool StorageLocal::createPartitionTable() {
    PedDevice *device = c_available_disks[c_settings->device_path()];
    
    std::cout << "starting to partition " << c_settings->device_path() << std::endl;
    
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
    
    c_disk = ped_disk_new_fresh(device, type);
    if( c_disk == nullptr ) {
      std::cerr << "Error while creating a new partition for " << c_settings->device_path() << std::endl;
      return false;
    }
    return true;
  }
  
  bool StorageLocal::addPartition ( unsigned int p_size, char p_size_unit, bool p_set_lvm_flag ) {
    if( c_disk == nullptr ) {
      return false;
    }
    
    // create new partition
    PedFileSystemType *fs_type = ped_file_system_type_get("ext2");
    if( fs_type == nullptr ) {
      std::cerr << "Error while getting fs type ext4 FILE/LINE" << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
        
    PedSector max_sectors, start_sector, part_end_sector;
    
    // get last usable sector
    max_sectors = c_available_disks[ c_settings->device_path() ]->length;
    
    // get start sector
    if( c_partitions.empty() ) {
      start_sector = 2048;
    } else {
      start_sector = c_partitions.back() + 1;
    }
    
    // get ending sector
    if( p_size == 0 ) {
      part_end_sector = max_sectors - 1;
    } else {
      part_end_sector = getSectorCounterForNumber(p_size, p_size_unit) + start_sector;
    }
    
    if( part_end_sector == 0 ) {
      std::cerr << "Error while calculating first_part_end " << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
        
    if( part_end_sector > max_sectors ) {
      std::cerr << "Error part_end_sector exeeded max_sector: " << part_end_sector << "tried and max: " << max_sectors << "max " << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
        
    PedPartition *part = ped_partition_new(c_disk, PED_PARTITION_NORMAL, fs_type, start_sector, part_end_sector);
    
    if( part == nullptr ) {
      ped_disk_print(c_disk);
      std::cerr << "Error while creating partition number " << c_partitions.size() << ' '
                << " max_sectors: " << max_sectors << " part_end_sector: " << part_end_sector
                << " start_sector: " << start_sector
                << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    if( p_set_lvm_flag && ped_partition_is_flag_available( part, PED_PARTITION_LVM ) == 1 ) {
      ped_partition_set_flag( part, PED_PARTITION_LVM, 1 );
    }
    
    PedConstraint *constraint = ped_device_get_constraint( c_available_disks[ c_settings->device_path() ] );
    
    if( ped_disk_add_partition(c_disk, part, constraint) == 0 ) {
      std::cerr << "Error while adding first partition" << __FILE__ << '/' << __LINE__ << std::endl;
      return false;
    }
    
    c_partitions.push_back( part_end_sector );
    
    return true;
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
    
    return (PedSector) return_value / c_available_disks[ c_settings->device_path() ]->sector_size;
  }



  unsigned long long StorageLocal::calculateSize ( PedCHSGeometry p_geometry, long p_sector_size ) {
    unsigned long long size = p_geometry.cylinders * p_geometry.heads * p_geometry.sectors * p_sector_size;
    
    // now calculate down to GB        KiB    MiB    GiB
    size = (unsigned long long) size / 1024 / 1024 / 1024;
    return size;
  }



  
}} // cloudos::tools
