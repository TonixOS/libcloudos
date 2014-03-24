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



#ifndef CLOUDOS_SYSTEM_INSTALLER_EXTENDED_HPP__
#define CLOUDOS_SYSTEM_INSTALLER_EXTENDED_HPP__

#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <cloudos/system/InstallerCoreSystem.hpp>
#include <cloudos/system/HDDisk.hpp>
#include <cloudos/tools/IPAddress.hpp>

// our configuration storage
#include <cloudos/proto/OS.Installer.pb.h>
#include <cloudos/proto/OS.System.pb.h>
#include <cloudos/proto/OS.Network.pb.h>
#include <cloudos/proto/OpenStack.NeutronServer.pb.h>

namespace cloudos {
namespace system {
  
  /**
   * Why we need this class?
   * Extends the system::Installer class. Merges functionality, needed
   * by InstallerHostSystem and InstallerManagementSystem
   * 
   * What is possible with this class?
   * Create an extended system (Cloud OS Host/Management)
   * 
   * Is there a special behaviour, on special circumstances?
   * Non known
   * 
   * How to use this class as an object?
   * class InstallerHostSystem : public InstallerExtended {
   * // some stuff
   * };
   */
  class InstallerExtended : public Installer {
  public:
    
    /**
     * We are requirering a Core installation, on which we are based on.
     * 
     * Use this method to set override the default (setting up our own Core installation)
     */
    bool setCoreSystem( InstallerCoreSystemPointer p_core );
    
    /**
     * You are able to set a valid HDDisk by yourself,
     * so you can create a custom HDDisk layout by yourself...
     * 
     * You should not use setConfigStorage(), as this would override setHarddisk()!
     */
    bool setHarddisk( HDDiskPointer p_disk );
    
    /**
     * Set the network NIC configuration
     */
    bool setConfigNetwork(boost::shared_ptr< config::os::NetworkInterface > p_config);
    
    /**
     * TODO comment
     */
    bool setConfigSystem(boost::shared_ptr< config::os::System > p_config);
    
    /**
     * TODO comment
     */
    bool setConfigInstaller(boost::shared_ptr< config::os::Installer > p_config);
    
    /**
     * TODO comment
     */
    virtual bool setConfigStorage(HDDisk::ConfigPointer p_config);
    
  protected:
    
    /**
     * The disk, we are installing in...
     * 
     * Might ne nullptr, so we won't use any harddrive...
     * and are installing in a directory
     */
    HDDiskPointer c_disk;
    
    /**
     * our source installation template
     */
    InstallerCoreSystemPointer c_core_installation;
    
    /**
     * CONFIGS
     * TODO write documentation
     * TODO clean up configuration objects
     */
    boost::shared_ptr< config::os::NetworkInterface >     c_config_network;
    boost::shared_ptr< config::os::System >               c_config_system;
    boost::shared_ptr< config::os::Installer >            c_config_installer;
    
    InstallerExtended ( const std::string& p_root_dir ) : Installer( p_root_dir ) {}
    InstallerExtended ( const fs::path& p_root_dir ) : Installer( p_root_dir ) {}
    InstallerExtended ( std::string && p_root_dir ) : Installer( std::move(p_root_dir) ) {}
    InstallerExtended ( fs::path && p_root_dir ) : Installer( std::move(p_root_dir) ) {}
    
    /**
     * Will check, if we need to copy data from a InstallerCoreSystem template
     * or if we need to build our core system by our own...
     */
    bool stepPrepareCoreSystem();
    
    /**
     * Sets up boot/grub/grub.cfg, if c_disk is set
     * 
     * You have to add this step by yourself! It won't be executed automaticly!
     */
    bool stepConfigureGrub();
    
    /**
     * Installs grub on our c_disk. Requires a valid c_disk!
     * Systems has to be installed on the destination c_disk! (RootPath == on c_disk mountpoint)
     * 
     * You have to add this step by yourself! It won't be executed automaticly!
     */
    bool stepInstallGrub();
    
    /**
     * Sets up etc/fstab, if c_disk is set
     * 
     * You have to add this step by yourself! It won't be executed automaticly!
     */
    bool stepConfigureFSTab();
    
    /**
     * Will configure the nameserver settings
     */
    bool stepConfigureNameserver();
    
    /**
     * Configure hostname
     */
    bool stepConfigureHostname();
    
    /**
     * Configures the root password, configured in config::os::System::root_password
     */
    bool stepSetRootPassword();
    
    /**
     * Enables the cloudos-postinstall systemd service
     */
    bool stepEnablePostInstall();
    
    /**
     * Some cleaning job... e.g. remove installer rpms
     */
    virtual bool stepCleanUpSetup();
    
    /**
     * We added an option, to use c_core_installation, instead of a tar-file object
     */
    virtual void checkIsValid();
    
  private:
  };
  
}}

#endif