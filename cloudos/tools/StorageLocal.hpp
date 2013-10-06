
#ifndef CLOUDOS_TOOLS_STORAGE_LOCAL_HPP__
#define CLOUDOS_TOOLS_STORAGE_LOCAL_HPP__

#include <set>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

#include <cloudos/tools/System.hpp>
#include <cloudos/proto/OS.Installer.pb.h>

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
    virtual const std::set<StorageLocalConfigPointer> getAvailableDisks();
    
    /**
     * Will apply all settings within c_settings on this system
     *   - create partition table
     *   - format partitions
     */
    virtual bool applyToSystem();
    
    
  private:
    StorageLocalConfigPointer c_settings;
    std::map<std::string, PedDevice*> c_available_disks;
    
    /**
     * returns size in GiB
     */
    unsigned long long calculateSize( PedCHSGeometry p_geometry, long p_sector_size );
    
    /**
     * Will return the number of sectors for this disk to match the nearest amount of disk-space
     * 
     * allowed units: M G
     * 
     * Will return 0 if there is an error
     */
    PedSector getSectorCounterForNumber( unsigned int p_value, char p_unit = 'G' );
    
    bool createPartitionTable();
    bool createPartitions();
  };
  
}
}

#endif
