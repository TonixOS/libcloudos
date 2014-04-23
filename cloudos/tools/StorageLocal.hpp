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



#ifndef CLOUDOS_TOOLS_STORAGE_LOCAL_HPP__
#define CLOUDOS_TOOLS_STORAGE_LOCAL_HPP__

#include <set>
#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include <cloudos/tools/System.hpp>
#include <OS_Installer.pb.h>

// access to local disks
extern "C" {
#include <parted/device.h>
}

namespace fs = boost::filesystem;

namespace cloudos {
namespace tools {
  
  class StorageLocal;
  
  typedef boost::shared_ptr<StorageLocal> StorageLocalPointer;
  typedef boost::shared_ptr<config::os::InstallerDisk> StorageLocalConfigPointer;
  
  class StorageLocal {
  public:
    StorageLocal();
    ~StorageLocal();
    
    virtual bool setSettings( const fs::path& p_file );
    virtual bool setSettings( StorageLocalConfigPointer p_settings );
    
    /**
     * Will return a pointer to an object of the copied internal storage information
     * This object is a merge of data, givven by the user, and our default values, givven
     * while constructing this object.
     * 
     * You will need to destroy the object by your own...
     */
    virtual StorageLocalConfigPointer getSettings();
    
    /**
     * returns a list of devices, without partitions!
     */
    virtual const std::set<StorageLocalConfigPointer> getAvailableDisks(bool p_filter_installable = false);
    
    /**
     * Will apply all settings within c_settings on this system
     *   - create partition table
     *   - format partitions
     */
    virtual bool applyToSystem();
    
    /**
     * will create a new partitiontable, based on disk size, it will
     * automaticly use GPT (if size > 2TB) or MSDOS
     * 
     * will override the current partitiontable on the device (if already available)
     */
    virtual bool createPartitionTable();
    
    /**
     * adds a partition to the disk
     * specify p_size==0 to allocate all available disk space
     */
    virtual bool addPartition(unsigned int p_size,
                              char p_size_unit = 'G',
                              bool p_set_lvm_flag = false,
                              const std::string p_fs_type = std::string("ext2"));
    
    /**
     * Will return "gpt" or "msdos", depending on the disk-size.
     */
    std::string getRequiredPartitionType() const;
    
  private:
    StorageLocalConfigPointer c_settings;
    std::map<std::string, PedDevice*> c_available_disks;
    
    /**
     * all partitions and their end-sectors
     */
    std::vector<PedSector> c_partitions;
    
    PedDisk *c_disk;
    
    /**
     * returns size in GiB
     */
    uint64_t calculateSize( PedCHSGeometry p_geometry, long p_sector_size );
    
    /**
     * Will return the number of sectors for this disk to match the nearest amount of disk-space
     * 
     * allowed units: M G
     * 
     * Will return 0 if there is an error
     */
    PedSector getSectorCounterForNumber( unsigned int p_value, char p_unit = 'G' );
  };
  
}
}

#endif
