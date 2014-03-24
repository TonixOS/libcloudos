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



#include <cloudos/system/HDDisk.hpp>

#include <string>

// may be removed, if removeExistingLVMPartitions() works...
// #include <boost/algorithm/string/classification.hpp>
// #include <boost/algorithm/string/trim.hpp>
// #include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>

#include <cloudos/system/Command.hpp>

namespace fs = boost::filesystem;

namespace cloudos {
namespace system {
  
  HDDisk::HDDisk() {
    c_device = nullptr;
    c_disk   = nullptr;
  }
  
  HDDisk::~HDDisk() {
  }
  
  bool HDDisk::createPartitionTable() {
    checkIsValid();
    
    if( reportInvalidObjectError( __PRETTY_FUNCTION__ ) ) {
      return false;
    }
    
    if( removeExistingLVMPartitions() == false ) {
      return false;
    }
    
    LOG_I() << "start creating a partition table for " << c_config->index();
    
    if( c_device == nullptr ) {
      c_device = ped_device_get( c_config->index().c_str() );
    }
    
    PedDiskType *type = ( c_config->size() > 2000 ) // if gt 2 TB, use GPT
                        ? ped_disk_type_get("gpt")
                        : ped_disk_type_get("msdos");
    
    if( type == nullptr ) {
      LOG_E() << "Error while getting partition type... Abort!";
      return false;
    }
    
    c_disk = ped_disk_new_fresh(c_device, type);
    if( c_disk == nullptr ) {
      LOG_E() << "Error while creating a new partition... Abort!";
      return false;
    }
    
    LOG_I() << "SUCCESS!";
    
    return true;
  }
  
  bool HDDisk::createPartitions() {
    checkIsValid();
    
    if( reportInvalidObjectError( __PRETTY_FUNCTION__ ) ) {
      return false;
    }
    
    LOG_I() << "creating partitions for dev " << c_config->index();
    
    // call createPartitionTable() if not already done!
    if( c_device == nullptr && createPartitionTable() == false ) {
      LOG_E() << "createPartitionTable() failed... Abort!";
      return false;
    }
    
    PedSector part_sector_start, part_sector_end, part_sector_max;
    part_sector_start = 2048; // starting by 2048, so the bootloader has some space
    part_sector_max   = c_device->length - 1; // -1 to fix some disks length, now we are same to use sector_max
    part_sector_end   = 0;
    
    for( int pos = 0, pos_max = c_config->partition_size(); pos < pos_max; ++pos) {
      config::os::hw::HDDiskPartition* part_config = c_config->mutable_partition(pos);
      
      LOG_I() << "creating partition " << pos+1;
      
      if( part_sector_start == part_sector_max ) {
        LOG_E() << "requested to create an additional partition, but there is no space left on the device... Abort!";
        return false;
      }
      
      // always use ext2, as LVM will be set by a flag
      PedFileSystemType* fs_type = ped_file_system_type_get("ext2");
      if( fs_type == nullptr ) {
        LOG_E() << "Error while getting fs type from ext2... Abort!";
        return false;
      }
      
      // calculate end sector
      part_sector_end = ( part_config->size() == 0 )
                        ? part_sector_max
                        : getSectorCounterForNumber( part_config->size(), part_config->size_unit().at(0) ) + part_sector_start;
      
      // prevent overcommiting
      if( part_sector_end > part_sector_max ) {
        LOG_W() << "end sector exeeded sector_max... downsizing sector_end to sector_max...";
        LOG_W() << "sector_end is at " << part_sector_end;
        LOG_W() << "sector_max is at " << part_sector_max;
        part_sector_end = part_sector_max;
      }
      
      PedPartition* part;
      part = ped_partition_new(c_disk, PED_PARTITION_NORMAL, fs_type, part_sector_start, part_sector_end);
      if( part == nullptr ) {
        ped_disk_print( c_disk );
        LOG_E() << "failed to create partition! Abort!";
        LOG_E() << "sector_start: " << part_sector_start << " sector_end: " << part_sector_end
                << "sector_max: " << part_sector_max;
        return false;
      }
      
      // check, if we need to set the LVM flag
      if( part_config->type() == config::os::hw::HDDiskPartition::LVM ) {
        if( ped_partition_is_flag_available( part, PED_PARTITION_LVM ) != 1 ) {
          LOG_E() << "flag LVM required, but device does not support it... Abort!";
          return false;
        }
        ped_partition_set_flag( part, PED_PARTITION_LVM, 1 );
      }
      
      // prepare adding the partition to our device...
      PedConstraint *constraint = ped_device_get_constraint( c_device );
      if( ped_disk_add_partition(c_disk, part, constraint) == 0 ) {
        LOG_E() << "failed to add partition to our device... Abort!";
        return false;
      }
      
      // now get the filesystem path
      part_config->set_index( ped_partition_get_path( part ) );
      
      LOG_I() << "SUCCESS: partition created with sector_start=" << part_sector_start
              << " sector_end=" << part_sector_end << " and size=" << part_config->size() << "G"
              << " path=" << part_config->index();
      // now prepare our next partition
      part_sector_start = part_sector_end + 1;
    }
    
    // now commit our partition layout to disk:
    if( ped_disk_commit_to_dev(c_disk) == 0 ) {
      LOG_E() << "commiting device partition layout to disk failed... Abort!";
      return false;
    }
    
    if( ped_disk_commit_to_os(c_disk) == 0 ) {
      LOG_E() << "commiting device partition layout to OS failed... Abort!";
      return false;
    }
    
    return true;
  }
  
  bool HDDisk::formatPartitions() {
    checkIsValid();
    
    if( reportInvalidObjectError( __PRETTY_FUNCTION__ ) ) {
      return false;
    }
    
    LOG_I() << "format partitions on device " << c_config->index();
    
    // call createPartitionTable() if not already done!
    if( c_device == nullptr && createPartitions() == false ) {
      LOG_E() << "createPartitions() failed... Abort!";
      return false;
    }
    
    for( auto& part_config : c_config->partition() ) {
      LOG_I() << "create filesystem " << part_config.filesystem() << " on " << part_config.index();
      
      fs::path part_path( part_config.index() );
      if( fs::exists( part_path ) == false ) {
        LOG_E() << "Partition path " << part_path.string() << " does not exist... Abort!";
        return false;
      }
      
      Command mkfs;
      if( part_config.filesystem().compare("swap") == 0 ) {
        mkfs << "mkswap" << part_config.index();
      } else {
        mkfs << "mkfs" << "-q" << "-t" << part_config.filesystem() << part_config.index();
      }
      
      if( mkfs.waitUntilFinished() != 0 ) {
        LOG_E() << "creating filesystem FAILED... Abort!";
        return false;
      }
      
    }
    return true;
  }
  
  uint64_t HDDisk::calculateSize ( PedCHSGeometry p_geometry, long p_sector_size ) {
    // now calculate down to GB form Byte
    // divide by 1024 == shift 10 bits to right
    return (p_geometry.cylinders * p_geometry.heads * p_geometry.sectors * p_sector_size) >> 30;
  }
  
  bool HDDisk::setDevice ( PedDevice* p_device ) {
    if( p_device == nullptr ) {
      LOG_W() << "invalid PedDevice given... Abort!";
      return false;
    }
    
    if( c_device != nullptr ) {
      LOG_W() << "overwriting c_device " << c_device->path
              << " with p_device "       << p_device->path
              << "... okay, okay; will go on, but you are doing strange things, man!";
      ped_device_close( c_device );
    }
    
    c_device = p_device;
    return true;
  }
  
  // 
  // P R O T E C T E D
  // 
  bool HDDisk::removeExistingLVMPartitions() {
    
    // use the "bash" wrapper, as calling vgchange directly won't work...
    // reason is unknown...  got the error: "command unknown, try "help""
    // which seems to come from vgchange directly
    Command vgchange("bash");
    vgchange << "-xc" << "vgchange -an";
    
    if( vgchange.waitUntilFinished() != 0 ) {
      LOG_E() << "failed to disable all logical volumes... Abort!";
      LOG_E() << "need to disable them, so we could create a clean partition table...";
      return false;
    }
    
    return true;
    
    /*
    LOG_I() << "searching for existing LVM volumes...";
    
    // FIXME: what if we got an LVM volume in an LVM volume?
    
    // 
    // get list of volumes
    // 
    
    // should output something like: /dev/vdb3:vg_service_data
    Command pvs;
    pvs << "pvs" << "--noheadings" << "--separator" << ":" << "--options" << "pv_name,vg_name";
    if( pvs.waitUntilFinished() != 0 ) {
      LOG_E() << "faild to get list of LVM partitions... Abort!";
      return false;
    }
    
    std::string pvs_output = pvs.getStdoutOutput().str();
    boost::trim(pvs_output);
    
    std::vector<std::string> pvs_lines, lvm_volumes, split_volumens;
    boost::split(pvs_lines, pvs_output, boost::is_any_of("\n"), boost::algorithm::token_compress_on);
    
    // prepare device check
    std::size_t path_length = c_config->index().length();
    
    // check available volumes
    for( auto& line : pvs_lines ) {
      boost::trim(line);
      if( line.substr(0, path_length) == c_config->index() ) {
        boost::split(split_volumens, line, boost::is_any_of(":"));
        if( split_volumens.size() < 2 ) {
          LOG_W() << "pvs line seems to be unuseable: " << line << " won't use it... Abort!";
          return false;
        }
        lvm_volumes.push_back( split_volumens[1] );
        split_volumens.clear();
      }
    }
    
    // remove matching volumes
    for( auto& volume : lvm_volumes ) {
      Command vgremove;
      vgremove << "vgremove" << "-f" << volume;
      if( vgremove.waitUntilFinished() != 0 ) {
        LOG_E() << "volume group could not be removed... Abort!";
        return false;
      }
      LOG_I() << "volume group " << volume << " removed!";
    }
    
    return true;*/
  }
  
  void HDDisk::checkIsValid() {
    if( c_config->has_index() == false ) {
      LOG_D() << "index is not configured...";
      c_is_valid = false;
      return;
    }
    fs::path path( c_config->index() );
    if( fs::exists(path) == false ) {
      LOG_D() << "index does not exists... " << c_config->index() ;
      c_is_valid = false;
      return;
    }
    c_is_valid = true;
  }
  
  // 
  // P R I V A T E
  // 
  
  PedSector HDDisk::getSectorCounterForNumber ( unsigned int p_value, char p_unit ) {
    
    short shiftterator = 0;
    switch( p_unit ) {
      case 'E':
        ++shiftterator;
      case 'T':
        ++shiftterator;
      case 'G':
        ++shiftterator;
      case 'M':
        ++shiftterator;
      case 'K':
        ++shiftterator;
        break;
      default:
        LOG_E() << "invalid p_unit given: " << p_unit << "... Abort!";
        return 0;
    }
    // multiply by 1024 == shift 10 bits to left
    uint64_t return_value = p_value << ( 10 * shiftterator );
    
    return (PedSector) return_value / c_device->sector_size;
  }
  
}} // cloudos::system
