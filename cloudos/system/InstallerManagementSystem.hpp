
#ifndef CLOUDOS_SYSTEM_INSTALLER_MANAGEMENT_SYSTEM_HPP__
#define CLOUDOS_SYSTEM_INSTALLER_MANAGEMENT_SYSTEM_HPP__

#include <vector>

#include <boost/container/stable_vector.hpp>

#include <cloudos/system/InstallerExtended.hpp>
#include <cloudos/proto/OpenStack.KeystoneClient.pb.h>

namespace cloudos {
namespace system {
  
  class InstallerManagementSystem;
  typedef boost::shared_ptr<InstallerManagementSystem> InstallerManagementSystemPointer;
  
  /**
   * Why we need this class?
   * This class is able to install a management system
   * 
   * What is possible with this class?
   * Create an extended system (Cloud OS Management)
   * 
   * Is there a special behaviour, on special circumstances?
   * Non known TODO TODO TODO TODO
   * 
   * How to use this class as an object?
   * InstallerManagementSystem mgtsys("/tmp/path/to/install/");
   * mgtsys.setCoreSystem( InstallerCoreSystemPointer );
   * mgtsys.setHarddisk( HDDiskPointer );
   * mgtsys.setup();
   */
  class InstallerManagementSystem : public InstallerExtended {
  public:
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerManagementSystem(const std::string& p_root_dir);
    
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerManagementSystem(std::string&& p_root_dir);
    
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerManagementSystem(fs::path&& p_root_dir);
    
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerManagementSystem(const fs::path& p_root_dir);
    
    /**
     * Calls zypper to install our packages
     */
    bool stepInstallMgtPackages();
    
    /**
     * Set up our firewall
     */
    bool stepConfigureFirewall();
    
    /**
     * Will set the network settings.
     */
    bool stepConfigureNetwork();
    
    /**
     * Will create the /etc/openstack/shell.config.sh file
     */
    bool stepConfigureHorizon();
    
    /**
     * Will create a c_disk object and
     * set up filesystems on it.
     * 
     * Also, it will mount our new root filesystem and reset RootPath
     * to the new mountpoint.
     * 
     * After this step, c_disk is valid!
     */
    bool stepSetupDiskImage();
    
    /**
     * Unmounts the install base and runs "qemu-img convert" on it,
     * to convert the image from raw to qcow2.
     */
    bool stepConvertImage();
    
    /**
     * Returns the path, of the converted and finished qcow2 image.
     * The path is only useful, after setup() was called
     */
    fs::path getImagePath() const;
    
  protected:
    
    boost::shared_ptr<config::openstack::KeystoneClient> c_config_keystone_client;
    
    /**
     * Contains the path to our converted qcow2 image if available,
     * else, it's empty...
     */
    fs::path c_image_path_qcow2;
    
    /**
     * Contains the path to our raw image, after it was copied to RAM...
     */
    fs::path c_image_path_raw;
    
    /**
     * Inits all class members...
     */
    virtual void init() override;
    
    /**
     * We just need c_root_dir to be valid, so override
     */
    virtual void checkIsValid() override;
        
    /**
     * We need to add our partition-layout
     * 
     * DISABLED: management needs its raw image template... so disable this function...
     */
    virtual bool setConfigStorage ( core::Config< HDDiskConfig >::ConfigPointer p_config ) { return false; }
    
  private:
  };
}}

#endif
