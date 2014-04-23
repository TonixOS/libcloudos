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



#include <cloudos/system/InstallerManagementSystem.hpp>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace cloudos {
namespace system {
  
  InstallerManagementSystem::InstallerManagementSystem(const std::string& p_root_dir) : InstallerExtended(p_root_dir) {
    init();
  }

  InstallerManagementSystem::InstallerManagementSystem(std::string && p_root_dir) : InstallerExtended(p_root_dir) {
    init();
  }

  InstallerManagementSystem::InstallerManagementSystem(fs::path && p_root_dir) : InstallerExtended( p_root_dir ) {
    init();
  }

  InstallerManagementSystem::InstallerManagementSystem(const fs::path& p_root_dir) : InstallerExtended(p_root_dir) {
    init();
  }

  bool InstallerManagementSystem::stepInstallMgtPackages() {
    if( searchForRPMsInDirectory("management") == false ) {
      LOG_E() << "management RPMs are required! Abort!";
      return false;
    }
    
    if( searchForRPMsInDirectory("openstack") == false ) {
      LOG_E() << "openstack RPMs are required! Abort!";
      return false;
    }
    
    return installPackages();
  }
  
  bool InstallerManagementSystem::stepConfigureFirewall() {
    if( c_config_network->ip_cidr_size() < 1 ) {
      LOG_E() << "not IP address set... Abort!";
      return false;
    }
    
    tools::IPAddress mgt_ip( c_config_network->ip_cidr(0) );
    if( mgt_ip.isValid() == false ) {
      LOG_E() << "Management IP is invalid... Abort!";
      return false;
    }
    
    std::stringstream ssv4, ssv6;
    ssv4 << "*filter\n"
         << ":INPUT DROP [0:0]\n"
         << ":FORWARD ACCEPT [0:0]\n"
         << ":OUTPUT ACCEPT [0:0]\n"
         << "-I INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT\n"
         << "-A INPUT -d " << mgt_ip.ip() << " -p tcp --dport 80 -j ACCEPT\n"
         << "-A INPUT -i lo -j ACCEPT\n"
         << "-A INPUT -p icmp -j ACCEPT\n"
         << "-A INPUT -s -p tcp --dport 22 -j ACCEPT\n"
         << "COMMIT";
    
    ssv6 << "*filter\n"
         << ":INPUT DROP [0:0]\n"
         << ":FORWARD ACCEPT [0:0]\n"
         << ":OUTPUT ACCEPT [0:0]\n"
         << "[0:0] -A INPUT -p icmp -j ACCEPT\n"
         << "COMMIT";
    
    File fwv4( getRootDirAsPath() / "etc/network/firewall/ipv4.rules" );
    File fwv6( getRootDirAsPath() / "etc/network/firewall/ipv6.rules" );
    return fwv4.setContent( std::move(ssv4) ) && fwv6.setContent( std::move(ssv6) );
  }
  
  bool InstallerManagementSystem::stepConfigureNetwork() {
    tools::IPAddress origin, gateway;
    origin.setValue( c_config_network->ip_cidr(0) );
    
    // our gateway is assumed to be our first ip in our network segment
    gateway.setValue( origin.netaddress() + "/" + origin.prefix() );
    gateway.setIncrementValue(1);
    
    File interface( getRootDirAsPath() / "etc/network/circuits/eth0.interface" );
    
    std::stringstream ss;
    ss << "IP=" << origin.cidr() << "\n"
    << "GATEWAY=" << gateway.ip() << "\n";
    
    if( interface.setContent( std::move(ss) ) == false ) {
      LOG_E() << "configuring NIC failed... Abort!";
      return false;
    }
    
    Command enable_nic("systemctl");
    enable_nic.setChRootController(c_chroot);
    enable_nic << "enable" << "network@eth0.service";
    
    return enable_nic.waitUntilFinished() == 0;
  }
  
  bool InstallerManagementSystem::stepConfigureHorizon() {
    File config( getRootDirAsPath() / "etc/openstack/horizon/setup_settings.py" );
    if( config.isWriteable() == false ) {
      return false;
    }
    
    std::stringstream ss;
    if( config.getContent( ss ) == false ) {
      return false;
    }
    
    LOG_I() << "replace __KEYSTONE_IP__ with " << c_config_keystone_client->host() << " in " << config.getFileName();
    std::string buf;
    boost::replace_all(buf, "__KEYSTONE_IP__", c_config_keystone_client->host());
    
    // generate SECRET
    // -a 0   => generate random passwords
    // -M NCL => only numbers, upper chars, lower chars
    // -n 10  => generate 10 passwords
    // -x 12  => max length = 12
    // -m 8   => min length = 8
    Command apg("apg");
    apg << "-a" << "0" << "-M" << "NCL" << "-n" << "1" << "-x" << "12" << "-m" << "8";
    if( apg.waitUntilFinished() != 0 ) {
      LOG_E() << "generating secret key failed... Abort!";
      return false;
    }
    
    std::string secret = apg.getStdoutOutput().str();
    boost::trim(secret);
    boost::replace_all(buf, "__SECRET_KEY__",  secret);
    
    return config.setContent( std::move(buf) );
    return false;
  }
  
  bool InstallerManagementSystem::stepSetupDiskImage() {
    File from_image( "/usr/share/cloudos/installer/management.template.qcow2.xz" );
    if( from_image.isAccessible() == false ) {
      return false;
    }
    
    // will load the module "loop", if not already done...
    Command modprobe("bash");
    modprobe << "-c" << "modprobe loop max_part=5 max_loop=2";
    
    if( modprobe.waitUntilFinished() != 0 ) {
      LOG_E() << "the loop module is required to set up the management system, but modprobe returned with code "
              << modprobe.getLastExitCode() << "... Abort!";
      return false;
    }
    
    // copy image to RAM
    fs::path tmp_img("/tmp/management.qcow2.xz");
    boost::system::error_code ec;
    fs::copy_file(from_image.getFileNameAsPath(), tmp_img, fs::copy_option::overwrite_if_exists, ec);
    if( ec ) {
      LOG_E() << "failed to copy our image template: " << ec.message() << " ...Abort!";
      return false;
    }
    
    // unpack image
    Command unxz("unxz");
    unxz << "-d" << tmp_img;
    if( unxz.waitUntilFinished() != 0 ) {
      LOG_E() << "unpack raw image template failed... Abort!";
      return false;
    }
    
    // convert image
    Command qemu_img("qemu-img");
    qemu_img << "convert" << "-f" << "qcow2" << "-O" << "raw" << "/tmp/management.qcow2" << c_image_path_raw;
    if( qemu_img.waitUntilFinished() != 0 ) {
      LOG_E() << "converting image template failed... Abort!";
      return false;
    }
    
    // configure our disk-layout
    typedef cloudos::config::os::hw::HDDiskPartition HDDPartition;
    HDDisk::ConfigPointer config( new HDDiskConfig() );
    config->set_index("/dev/loop0");
    HDDPartition* swap = config->add_partition();
    HDDPartition* root = config->add_partition();
    
    swap->set_index( "/dev/loop0p1" );
    swap->set_filesystem("swap");
    
    root->set_index( "/dev/loop0p2" );
    root->set_filesystem("ext4");
    
    // attach image to loop dev
    Command losetup("losetup");
    losetup << "--detach-all";
    if( losetup.waitUntilFinished() != 0 ) {
      LOG_E() << "detaching old sources from loop devs failed... Abort!";
      return false;
    }
    losetup.clearArguments();
    
    losetup << "--partscan" << config->index() << c_image_path_raw;
    if( losetup.waitUntilFinished() != 0 ) {
      LOG_E() << "attaching image to " << config->index() << " failed... Abort!";
      return false;
    }
    
    // okay, now init c_disk
    HDDiskPointer disk( new HDDisk() );
    PedDevice* device = ped_device_get( config->index().c_str() );
    
    if( disk->setDevice( device ) == false ) {
      LOG_E() << "could not init device " << config->index() << "... Abort!";
      return false;
    }
    
    if( disk->setConfig( config ) == false ) {
      LOG_E() << "failed to set configuration options to our disk... Abort!";
      return false;
    }
    
    if( disk->formatPartitions() == false ) {
      LOG_E() << "failed to format partitions... Abort!";
      return false;
    }
    
    Command mount("mount");
    mount << root->index() << getRootDir();
    if( mount.waitUntilFinished() != 0 ) {
      LOG_E() << "failed to mount management root partition to " << getRootDir() << "... Abort!";
      return false;
    }
    
    return true;
  }
  
  bool InstallerManagementSystem::stepConvertImage() {
    
    // disable chroot, if enabled
    if( c_chroot->isEnabled() && c_chroot->disable() == false ) {
      LOG_E() << "failed to disable chroot... Abort!";
      return false;
    }
    
    // umount root fs
    Command umount("umount");
    umount << getRootDir();
    if( umount.waitUntilFinished() != 0 ) {
      LOG_E() << "failed to umount root path... so we are unable to convert the image... Abort!";
      return false;
    }
    
    // detach loop device
    Command losetup("losetup");
    losetup << "-D";
    if( losetup.waitUntilFinished() != 0 ) {
      LOG_E() << "detaching the loop device failed... Abort!";
      return false;
    }
    
    // convert image
    Command qemu_img("qemu-img");
    qemu_img << "convert" << "-f" << "raw" << "-O" << "qcow2" << c_image_path_raw << c_image_path_qcow2;
    
    return false;
  }
  
  fs::path InstallerManagementSystem::getImagePath() const {
    return c_image_path_qcow2;
  }
  
  // 
  // P R O T E C T E D
  // 
  void InstallerManagementSystem::init() {
    LOG_I() << "loading steps...";
    appendInstallerStep( "setup_disk_img",   boost::bind(&InstallerManagementSystem::stepSetupDiskImage,      this) );
    appendInstallerStep( "prepare_coredata", boost::bind(&InstallerManagementSystem::stepPrepareCoreSystem,   this) );
    appendInstallerStep( "package_install",  boost::bind(&InstallerManagementSystem::stepInstallMgtPackages,  this) );
    appendInstallerStep( "network_conf",     boost::bind(&InstallerManagementSystem::stepConfigureNetwork,    this) );
    appendInstallerStep( "dns_conf",         boost::bind(&InstallerManagementSystem::stepConfigureNameserver, this) );
    appendInstallerStep( "hostname_conf",    boost::bind(&InstallerManagementSystem::stepConfigureHostname,   this) );
    appendInstallerStep( "firewall_conf",    boost::bind(&InstallerManagementSystem::stepConfigureFirewall,   this) );
    appendInstallerStep( "setup_fstab",      boost::bind(&InstallerManagementSystem::stepConfigureFSTab,      this) );
    appendInstallerStep( "horizon_conf",     boost::bind(&InstallerManagementSystem::stepConfigureHorizon,    this) );
    appendInstallerStep( "enable_postinst",  boost::bind(&InstallerManagementSystem::stepEnablePostInstall,   this) );
    appendInstallerStep( "cleanup",          boost::bind(&InstallerManagementSystem::stepCleanUpSetup,        this) );
    appendInstallerStep( "grub_conf",        boost::bind(&InstallerManagementSystem::stepConfigureGrub,       this) );
    appendInstallerStep( "convert_disk_img", boost::bind(&InstallerManagementSystem::stepConvertImage,        this) );
    LOG_I() << "steps loaded";
    
    c_image_path_raw = "/tmp/cloudos/installer/management.img.raw";
  }
  
  void InstallerManagementSystem::checkIsValid() {
    cloudos::system::InstallerExtended::checkIsValid();
  }
  
}}
