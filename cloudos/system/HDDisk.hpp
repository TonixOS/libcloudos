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



#ifndef CLOUDOS_SYSTEM_HDDISK_HPP__
#define CLOUDOS_SYSTEM_HDDISK_HPP__

#include <boost/smart_ptr/shared_ptr.hpp>

#include <OS_HW_HDDisk.pb.h>
#include <cloudos/core/Config.hpp>

// access to local disks
extern "C" {
#include <parted/device.h>
}

namespace cloudos {
namespace system {
  
  class HDDisk;
  typedef boost::shared_ptr<HDDisk> HDDiskPointer;
  typedef config::os::hw::HDDisk    HDDiskConfig;
  
  /**
   * Why we need this class?
   * Manage a local hard disk.
   * 
   * What is possible with this class?
   * Create a partition layout and format the partitions.
   * 
   * Is there a special behaviour, on special circumstances?
   * It will decline to overwrite the currently running OS disk!
   * So, if your "/" is mounted by /dev/vdb1, it won't do anything to
   * /dev/vdb! Be aware of this!
   * 
   * How to use this class as an object?
   * HDDisk disk;
   * HDDisk::ConfigPointer config;
   * config->set_device_path("/dev/vdc");
   * // add partitions to "config"
   * disk.setConfig( config );
   * 
   * if( disk.isValid() ) {
   *   // create partition layout, you won't need this step, if the wanted layout already exists
   *   disk.createPartitionTable();
   *   disk.formatPartitions();
   * }
   */
  class HDDisk : public core::Config<HDDiskConfig> {
  public:
    HDDisk();
    ~HDDisk();
    
    /**
     * Creates a new partition table and the corresponding partitions on the c_config disk.
     * Will overwrite the existing partition table on the disk, all your data on this disk
     * W I L L   B E   L O S T!
     * 
     * Requires isValid() == true
     */
    bool createPartitionTable();
    
    /**
     * Applies filesystems on the created/loaded partition-layout.
     * Will overwrite all existing filesystems on the disk, all your data on this disk
     * W I L L   B E   L O S T!
     * 
     * If there is no partition table on the device, createPartitions() will call createPartitionTable().
     * 
     * Requires isValid() == true
     */
    bool createPartitions();
    
    /**
     * Will create a new filesystem on all given partitions...
     */
    bool formatPartitions();
    
    /**
     * returns size in GiB
     */
    static uint64_t calculateSize( PedCHSGeometry p_geometry, long p_sector_size );
    
    /**
     * Set a PedDevice* to use this->formatPartitions() directly...
     * formatPartitions() requires a vaild PedDevice!
     */
    bool setDevice( PedDevice* p_device );
    
  protected:
    
    /**
     * Removes existing LVM volumes from the disk layout, so we
     * are able to create a new partition-table.
     */
    bool removeExistingLVMPartitions();
    
    /**
     * will control c_is_valid
     */
    virtual void checkIsValid() override;
    
  private:
    
    /**
     * Our device
     */
    PedDevice* c_device;
    
    /**
     * Our disk
     */
    PedDisk* c_disk;
    
    /**
     * Will return the number of sectors for this disk to match the nearest amount of disk-space
     * 
     * allowed units: M G
     * 
     * Will return 0 if there is an error
     */
    PedSector getSectorCounterForNumber( unsigned int p_value, char p_unit = 'G' );
  };
  
}} // cloudos::system

#endif
