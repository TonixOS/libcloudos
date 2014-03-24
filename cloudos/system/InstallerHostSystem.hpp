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



#ifndef CLOUDOS_SYSTEM_INSTALLER_HOST_SYSTEM_HPP__
#define CLOUDOS_SYSTEM_INSTALLER_HOST_SYSTEM_HPP__

#include <vector>

#include <boost/container/stable_vector.hpp>
#include <boost/thread.hpp>

#include <cloudos/system/InstallerExtended.hpp>
#include <cloudos/system/InstallerManagementSystem.hpp>
#include <cloudos/proto/OpenStack.NeutronServer.pb.h>

namespace cloudos {
namespace system {
  
  class InstallerHostSystem;
  typedef boost::shared_ptr<InstallerHostSystem> InstallerHostSystemPointer;
  typedef boost::shared_ptr<boost::thread>       ThreadPointer;
  
  /**
   * Why we need this class?
   * This class is able to install a host system
   * 
   * What is possible with this class?
   * Create an extended system (Cloud OS Host)
   * 
   * Is there a special behaviour, on special circumstances?
   * Non known
   * 
   * How to use this class as an object?
   * InstallerHostSystem hostsys("/tmp/path/to/install/");
   * hostsys.setCoreSystem( InstallerCoreSystemPointer );
   * hostsys.setHarddisk( HDDPointer );
   * hostsys.setup();
   */
  class InstallerHostSystem : public InstallerExtended {
  public:
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerHostSystem(const std::string& p_root_dir);
    
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerHostSystem(std::string&& p_root_dir);
    
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerHostSystem(fs::path&& p_root_dir);
    
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerHostSystem(const fs::path& p_root_dir);
    
    /**
     * We need to add our partition-layout
     */
    virtual bool setConfigStorage ( core::Config< HDDiskConfig >::ConfigPointer p_config );
    
    /**
     * TODO comment
     */
    bool setConfigOpenStackNeutron(boost::shared_ptr< config::openstack::NeutronServer > p_config);
    
    /**
     * Set a management installer, if you want a management vm on the new host,
     * created by this installer.
     * 
     * p_management needs to be created and configured, before setting it!
     * 
     * The management will be installed by a new thread - step "mgt_setup_start"
     * will do the trick...
     * "mgt_setup_finish" will copy the converted management image to our c_disk
     */
    bool setManagementInstaller( InstallerManagementSystemPointer p_management );
    
    /**
     * Calls zypper to install our packages
     */
    bool stepInstallHostPackages();
    
    /**
     * Will disable service files, so the postinstall script can handle that.
     * FIXME: remove postinstall step! And then this step :-)
     */
    bool stepDisableServices();
    
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
    bool stepConfigureOpenStack();
    
    /**
     * Will move all data to our c_disk.
     * 
     * Sets RootDir to new c_disk / mountpoint
     * 
     * After this step, you can safely call stepInstallGrub()
     */
    bool stepMoveDataToDisk();
    
    /**
     * Creates the thread c_mgt_installer_thread from c_mgt_installer
     * if c_mgt_installer is set.
     */
    bool stepLaunchManagementInstaller();
    
    /**
     * Waits until the management installer finishes and copies the resulting image to our rootdir
     * Will be executed only if a management installer is set
     */
    bool stepFinishManagementInstaller();
    
  protected:
    
    /**
     * management installer to be able to control the
     * management installation and copy all data to our
     * c_disk...
     * 
     * set it with setManagementInstaller()
     */
    InstallerManagementSystemPointer c_mgt_installer;
    
    /**
     * Will be initiated by stepLaunchManagementInstaller()
     * if c_mgt_installer is set.
     */
    ThreadPointer c_mgt_installer_thread;
    
    /**
     * Inits all class members...
     */
    virtual void init() override;
    
    /**
     * We just need c_root_dir to be valid, so override
     */
    virtual void checkIsValid() override;
    
    /**
     * CONFIGS
     * TODO write documentation
     * TODO clean up configuration objects
     * TODO write setConfig() functions
     */
    boost::shared_ptr< config::openstack::NeutronServer > c_config_neutron;
    
  private:
  };
}}

#endif
