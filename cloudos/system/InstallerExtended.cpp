
#include <cloudos/system/InstallerExtended.hpp>

namespace cloudos {
namespace system {
  
  bool InstallerExtended::setCoreSystem ( InstallerCoreSystemPointer p_core ) {
    if( !p_core ) {
      LOG_F() << "got uninitialised InstallerCoreSystemPointer... Abort!";
      return false;
    }
    
    if( p_core->isValid() == false ) {
      LOG_E() << "could not use the given InstallerCoreSystem, as it is invalid! Abort!";
      return false;
    }
    
    LOG_T() << "set InstallerCoreSystem from " << p_core->getRootDir();
    
    c_core_installation = p_core;
    checkIsValid();
    return true;
  }
  
  bool InstallerExtended::setHarddisk ( HDDiskPointer p_disk ) {
    if( !p_disk ) {
      LOG_F() << "got uninitialised HDDiskPointer... Abort!";
      return false;
    }
    
    if( p_disk->isValid() == false ) {
      LOG_E() << "given p_disk (" << p_disk->getConfig()->index() << ") is invalid... Abort!";
      return false;
    }
    
    c_disk = p_disk;
    checkIsValid();
    return true;
  }
  
  bool InstallerExtended::setConfigNetwork ( boost::shared_ptr< config::os::NetworkInterface > p_config ) {
    if( !p_config ) {
      LOG_E() << "config object is INVALID (has no pointing element)... Abort!";
      return false;
    }
    
    c_config_network = p_config;
    checkIsValid();
    return true;
  }
  
  bool InstallerExtended::setConfigSystem ( boost::shared_ptr< config::os::System > p_config ) {
    if( !p_config ) {
      LOG_E() << "config object is INVALID (has no pointing element)... Abort!";
      return false;
    }
    
    bool remove_old_step = c_config_system && c_config_system->has_root_password();
    
    c_config_system = p_config;
    checkIsValid();
    
    if( remove_old_step ) {
      removeInstallerStep( "set_root_pw" );
    }
    
    if( c_config_system->has_root_password() ) {
      appendInstallerStep( "set_root_pw", boost::bind(&InstallerExtended::stepSetRootPassword, this) );
    }
    
    return true;
  }
  
  bool InstallerExtended::setConfigInstaller ( boost::shared_ptr< config::os::Installer > p_config ) {
    if( !p_config ) {
      LOG_E() << "config object is INVALID (has no pointing element)... Abort!";
      return false;
    }
    
    c_config_installer = p_config;
    checkIsValid();
    return true;
  }
  
    bool InstallerExtended::setConfigStorage ( HDDisk::ConfigPointer p_config ) {
    if( !p_config ) {
      LOG_E() << "config object is INVALID (has no pointing element)... Abort!";
      return false;
    }
        
    // remove steps only if we already got a c_disk set
    // and it was valid... (only then, we had added the steps...)
    if( c_disk && c_disk->isValid() ) {
      removeInstallerStep( "create_patition_table" );
      removeInstallerStep( "create_patitions" );
      removeInstallerStep( "format_patitions" );
    }
    
    LOG_D() << "add HDD configuration with device: " << p_config->index();
    c_disk = HDDiskPointer( new HDDisk() );
    c_disk->setConfig( p_config );
    
    if( c_disk->isValid() == false ) {
      LOG_E() << "c_disk is now invalid... Abort!";
      return false;
    }
        
    // insert in reverse order, as we are always pushing at the front...
    pushFrontInstallerStep( "format_patitions",      boost::bind(&HDDisk::formatPartitions,     c_disk.get()) );
    pushFrontInstallerStep( "create_patitions",      boost::bind(&HDDisk::createPartitions,     c_disk.get()) );
    pushFrontInstallerStep( "create_patition_table", boost::bind(&HDDisk::createPartitionTable, c_disk.get()) );
    
    checkIsValid();
    
    return true;
  }
  
  // 
  // P R O T E C T E D
  // 
  
  bool InstallerExtended::stepPrepareCoreSystem() {
    
    // create our own, if setCoreSystem() wasn't called by 
    if( !c_core_installation ) {
      LOG_I() << "creating our own InstallerCoreSystem instance...";
      
      if( !getFileTar() ) {
        LOG_E() << "no tar file set! Can't continue without tar file... Abort!";
        return false;
      }
      
      c_core_installation = InstallerCoreSystemPointer( new InstallerCoreSystem( getRootDir() ) );
      fs::path tar_file(getFileTar()->getFileNameAsPath());
      c_core_installation->setPackagesTarFile( std::move(tar_file) );
    }
        
    if( c_core_installation->isSetupFinished() == false && c_core_installation->setup() == false ) {
      LOG_E() << "could not create core installation... Abort!";
      return false;
    }
    
    if( c_core_installation->getRootDir() == getRootDir() ) {
      LOG_I() << "no need for data transfer, as we install the host system into the core system...";
      
      if( c_core_installation->disableChroot() == false ) {
        LOG_E() << "could not disable chroot from core installation... Abort!";
        return false;
      }
      
      return true;
    }
        
    Directory root_dir( getRootDirAsPath() );
    if( c_core_installation->replicateToDirectory( root_dir ) == false ) {
      LOG_E() << "could not copy data from our InstallerCoreSystem template... Abort!";
      return false;
    }
    
    return true;
  }
  
  bool InstallerExtended::stepConfigureGrub() {
    
    if( !c_disk ) {
      LOG_I() << "nothing to do... c_disk is not set! Return SUCCESS";
      return true;
    }
    
    Command mkconf("grub-mkconfig");
    mkconf.setChRootController( c_chroot );
    mkconf << "-o" << "/boot/grub/grub.cfg";
    
    if( mkconf.waitUntilFinished() != 0 ) {
      return false;
    }
    
    return true;
  }
  
  bool InstallerExtended::stepInstallGrub() {
    if( !c_disk ) {
      LOG_E() << "c_disk is not set, but required... Abort!";
      return false;
    }
    if( c_disk->isValid() == false ) {
      LOG_E() << "c_disk is invalid... Abort!";
      return false;
    }
    
    
    Command install("grub-install");
    install.setChRootController( c_chroot );
    install << "--debug" << c_disk->getConfig()->index();
    
    if( install.waitUntilFinished() != 0 ) {
      LOG_E() << "grub-install failed... Abort!";
      return false;
    }
    
    return true;
  }
    
  bool InstallerExtended::stepConfigureFSTab() {
    //
    //TODO: write an C++ FSTab object
    //FIXME: use protobuf config object for partitions
    //       and implement /proc and co template config, which will be merged with the disk config
    std::stringstream ss;
    ss << "# file system  mount-point  type     options                dump  fsck\n"
       << "#                                                                 order\n"
       << "\n";
    
    if( c_disk ) {
      HDDisk::ConfigPointer config = c_disk->getConfig();
      int part_num = 0, part_max = config->partition_size();
      for( ; part_num < part_max; ++part_num ) {
        const config::os::hw::HDDiskPartition& part = config->partition(part_num);
        
        if( part.filesystem().compare("swap") == 0 ) {
          ss << part.index() << " " << part.filesystem() << " " << part.filesystem() << " pri=1 0 0\n";
        } else {
          ss << part.index() << " " << part.mountpoint() << " " << part.filesystem() << " "
             << part.mountoptions() << " 1 1\n";
        }
      }
    }
   
    ss << "proc           /proc        proc     nosuid,noexec,nodev    0     0\n"
       << "sysfs          /sys         sysfs    nosuid,noexec,nodev    0     0\n"
       << "devpts         /dev/pts     devpts   gid=5,mode=620         0     0\n"
       << "tmpfs          /run         tmpfs    defaults               0     0\n"
       << "devtmpfs       /dev         devtmpfs mode=0755,nosuid       0     0\n";
    File fstab( getRootDir() + "/etc/fstab" );
    return fstab.setContent( std::move( ss ) );
  }
  
  bool InstallerExtended::stepConfigureNameserver() {
    File resolv_conf( getRootDirAsPath() / "etc/resolv.conf" );
    
    std::stringstream ss;
    ss << "nameserver 78.138.97.33\n"
    << "nameserver 78.138.98.82\n";
    
    return resolv_conf.setContent( std::move(ss) );
  }
    
  bool InstallerExtended::stepConfigureHostname() {
    File hostname( getRootDirAsPath() / "etc/hostname" );
    
    std::stringstream ss;
    ss << c_config_system->hostname();
    
    if( hostname.setContent( std::move(ss) ) == false ) {
      return false;
    }
    
    if( c_config_network->ip_cidr_size() < 1 ) {
      LOG_E() << "Requires at least one IP to be configured for interface " << c_config_network->index() << "Abort!";
      return false;
    }
    
    // configure our hosts conf
    tools::IPAddress ip;
    ip.setValue( c_config_network->ip_cidr(0) );
    
    if( ip.isValid() == false ) {
      LOG_E() << "the configured ip is invalid! Could not configure /etc/hosts... Abort!";
      return false;
    }
    
    File hosts( getRootDirAsPath() / "etc/hosts" );
    
    std::stringstream sshosts;
    sshosts << ip.ip() << " " << c_config_system->hostname() << "\n";
    
    return hosts.addContent( std::move(sshosts) );
  }
  
  bool InstallerExtended::stepSetRootPassword() {
    if( c_config_system->has_root_password() == false ) {
      LOG_E() << "root password is not configured... but step stepSetRootPassword is called... This should not happen... Abort!";
      return false;
    }
    
    Command passwd("passwd");
    passwd.setChRootController(c_chroot);
    
    std::stringstream ss;
    ss << c_config_system->root_password() << "\n"
       << c_config_system->root_password() << "\n";
    passwd.setInputStreamData( std::move(ss.str()) );
    
    return passwd.waitUntilFinished() == 0;
  }
  
  bool InstallerExtended::stepEnablePostInstall() {
    
    Command systemctl("systemctl");
    systemctl.setChRootController(c_chroot);
    systemctl << "enable" << "cloudos-postinstall.service";
    
    return systemctl.waitUntilFinished() == 0;
  }
  
  bool InstallerExtended::stepCleanUpSetup() {
    fs::remove_all( getRootDirAsPath() / "host" );
    fs::remove_all( getRootDirAsPath() / "openstack" );
    fs::remove_all( getRootDirAsPath() / "management" );
    fs::remove_all( getRootDirAsPath() / "installer" );
    return true;
  }
  
  void InstallerExtended::checkIsValid() {
    cloudos::system::Installer::checkIsValid();
    
    // if tar-file is set, ok! else, we need a valid c_core_installation object
    c_is_valid = c_is_valid || ( c_core_installation && c_core_installation->isValid() );
  }
  
}
}
