
#include <sstream>

#include <cloudos/system/InstallerHostSystem.hpp>

namespace cloudos {
namespace system {
  
  InstallerHostSystem::InstallerHostSystem ( const std::string& p_root_dir ) : InstallerExtended(p_root_dir) {
    init();
  }
  
  InstallerHostSystem::InstallerHostSystem(std::string && p_root_dir) : InstallerExtended(std::move(p_root_dir)) {
    init();
  }
  
  InstallerHostSystem::InstallerHostSystem ( fs::path && p_root_dir ) : InstallerExtended(std::move(p_root_dir)) {
    init();
  }
  
  InstallerHostSystem::InstallerHostSystem ( const fs::path& p_root_dir ) : InstallerExtended(p_root_dir) {
    init();
  }
  
  bool InstallerHostSystem::setConfigStorage ( core::Config< HDDiskConfig >::ConfigPointer p_config ) {
    
    config::os::hw::HDDiskPartition* swap = p_config->add_partition();
    swap->set_filesystem("swap");
    swap->set_size(500);
    swap->set_size_unit("MB");
    
    config::os::hw::HDDiskPartition* root = p_config->add_partition();
    root->set_filesystem("ext4");
    root->set_mountpoint("/");
    root->set_mountoptions("defaults,noatime");
    root->set_size(0); //FIXME split disk up to seperate /srv and root
    /*
    config::os::hw::HDDiskPartition* srv = p_config->add_partition();
    srv->set_type(config::os::hw::HDDiskPartition_PartType_LVM);
    srv->set_mountpoint("/srv");
    srv->set_mountoptions("defaults,noatime");
    srv->set_size(0); // all remaining space...
    */
    return cloudos::system::InstallerExtended::setConfigStorage ( p_config );
  }
  
  bool InstallerHostSystem::setConfigOpenStackNeutron ( boost::shared_ptr< config::openstack::NeutronServer > p_config ) {
    if( !p_config ) {
      LOG_E() << "config object is INVALID (has no pointing element)... Abort!";
      return false;
    }
    
    tools::IPAddress public_pool( p_config->public_ip_pool() );
    if( public_pool.isValid() == false ) {
      LOG_E() << "given public ip pool is invalid... Abort!";
      return false;
    }
    
    LOG_D() << "add neutron configuration with public ip pool: " << p_config->public_ip_pool();
    c_config_neutron = p_config;
    checkIsValid();
    
    return true;
  }
  
  bool InstallerHostSystem::setManagementInstaller ( InstallerManagementSystemPointer p_management ) {
    if( ! p_management ) {
      LOG_E() << "given installer is a nullptr... Abort!";
      return false;
    }
    
    if( c_mgt_installer ) {
      LOG_E() << "management installer is already set! (current: " << c_mgt_installer->getRootDir()
              << " given: " << c_mgt_installer->getRootDir() << " Abort!";
      return false;
    }
    
    c_mgt_installer = p_management;
    
    insertInstallerStep("launch_mgt_installer",boost::bind(&InstallerHostSystem::stepLaunchManagementInstaller,this), "prepare_coredata");
    appendInstallerStep("finish_mgt_installer",boost::bind(&InstallerHostSystem::stepFinishManagementInstaller,this));
    
    return true;
  }
  
  bool InstallerHostSystem::stepInstallHostPackages() {
    
    if( searchForRPMsInDirectory("host") == false ) {
      LOG_E() << "host RPMs are required! Abort!";
      return false;
    }
    
    if( searchForRPMsInDirectory("openstack") == false ) {
      LOG_E() << "openstack RPMs are required! Abort!";
      return false;
    }
    
    return installPackages();
  }
  
  bool InstallerHostSystem::stepDisableServices() {
    Directory service_dir( getRootDirAsPath() / "usr/lib/systemd/system" );
    
    if( service_dir.isAccessible() == false ) {
      LOG_E() << "services directory is not accessible... Abort!";
      return false;
    }
    
    std::vector<fs::path> results;
    service_dir.findFiles(results, "^(openstack|rabbitmq|openvswitch|mariadb).*\\.service$", LOOKUP_BY_REGEX);
    
    Command disable_services;
    disable_services.setChRootController( c_chroot );
    disable_services << "systemctl" << "disable";
    
    for( auto& service : results ) {
      disable_services.addArgument( service.filename().string() );
    }
    
    return disable_services.waitUntilFinished() == 0;
  }
  
  bool InstallerHostSystem::stepConfigureFirewall() {
    
    std::stringstream ssv4, ssv6;
    ssv4 << "*filter\n"
         << ":INPUT DROP [0:0]\n"
         << ":FORWARD ACCEPT [0:0]\n"
         << ":OUTPUT ACCEPT [0:0]\n"
         << "-I INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT\n"
         << "-A INPUT -s " << c_config_neutron->public_ip_pool() << " -j ACCEPT\n"
         << "-A INPUT -i lo -j ACCEPT\n"
         << "-A INPUT -p icmp -j ACCEPT\n"
         << "-A INPUT -p tcp --dport 22 -j ACCEPT\n"
         << "-A INPUT -p tcp --dport 6080 -m comment --comment \"allow access to VNC proxy\" -j ACCEPT\n"
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
  
  bool InstallerHostSystem::stepConfigureNetwork() {
    tools::IPAddress origin, gateway;
    origin.setValue( c_config_neutron->public_ip_pool() );
    
    // get the right base
    origin.setValue( origin.netaddress() + '/' + origin.prefix() );
    
    // our gateway
    gateway.setValue( origin.cidr() );
    gateway.setIncrementValue(1);
    
    // now set the host ip
    origin.setIncrementValue(2);
    
    File interface( getRootDirAsPath() / "etc/network/circuits/br-ex.interface" );
    
    std::stringstream ss;
    ss << "IP=" << origin.cidr() << "\n"
    << "GATEWAY=" << gateway.ip() << "\n";
    
    // prepare stepConfigureHostname()
    // FIXME: set valid c_config_network
    c_config_network->add_ip_cidr( origin.cidr() );
    
    if( interface.setContent( std::move(ss) ) == false ) {
      LOG_E() << "configuring NIC failed... Abort!";
      return false;
    }
    
    Command enable_nic("systemctl");
    enable_nic.setChRootController(c_chroot);
    enable_nic << "enable" << "network@br-ex.service";
    
    return enable_nic.waitUntilFinished() == 0;
  }
  
  bool InstallerHostSystem::stepConfigureOpenStack() {
    //FIXME: make postinstall deprecated
    tools::IPAddress ip_origin, ip_host, ip_pool_start, ip_pool_end, ip_mgt;
    
    ip_origin.setValue( c_config_neutron->public_ip_pool() );
    
    // get the right base
    ip_origin.setValue( ip_origin.netaddress() + '/' + ip_origin.prefix() );
    
    // our own host (server)
    ip_host.setValue( ip_origin.cidr() );
    ip_host.setIncrementValue(2);
    
    ip_mgt.setValue( ip_host.cidr() );
    ip_mgt.setIncrementValue(2);
    
    ip_pool_start.setValue( ip_host.cidr() );
    ip_pool_start.setIncrementValue(1);
    
    ip_pool_end.setValue( ip_origin.broadcast() );
    ip_pool_end.setIncrementValue(-1);
    
    //TODO: check ip objects, is they are valid!
    
    std::stringstream ss;
    ss << "CONFIG_MGT_MODE=\n"
       << "USER_ADMIN_NAME=operator\n"
       << "CONFIG_HOST_IP="           << ip_host.ip()       << "\n"
       << "CONFIG_IP_MGT="            << ip_mgt.ip()        << "\n"
       << "CONFIG_DBMS_HOST="         << ip_host.ip()       << "\n"
       << "CONFIG_KEYSTONE_IP="       << ip_host.ip()       << "\n"
       << "CONFIG_RABBITMQ_IP="       << ip_host.ip()       << "\n"
       << "CONFIG_GLANCE_IP="         << ip_host.ip()       << "\n"
       << "CONFIG_QUANTUM_IP="        << ip_host.ip()       << "\n"
       << "CONFIG_IP_POOL_START="     << ip_pool_start.ip() << "\n"
       << "CONFIG_IP_POOL_END="       << ip_pool_end.ip()   << "\n"
       << "CONFIG_KEYMAP="            << c_config_system->keyboard_layout() << "\n"
       << "CONFIG_PRIMARY_INTERFACE=" << c_config_network->index()          << "\n"
       << "CONFIG_PUBLIC_IP_POOL="    << c_config_neutron->public_ip_pool() << "\n";
    
    File shell_config( getRootDirAsPath() / "etc/openstack/shell.config.sh" );
    return shell_config.setContent( std::move(ss) );
  }
  
  bool InstallerHostSystem::stepMoveDataToDisk() {
    if( !c_disk ) {
      LOG_E() << "c_disk is not set, but a host-setup needs a valid disk... Abort!";
      return false;
    }
    if( c_disk->isValid() == false ) {
      LOG_E() << "c_disk is invalid, but a host-setup needs a valid disk... Abort!";
      return false;
    }
    
    // get root partition dev path
    std::string dev_path;
    HDDisk::ConfigPointer hdd_config = c_disk->getConfig();
    auto part = hdd_config->partition().begin(), end = hdd_config->partition().end();
    for(; part != end ; ++part) {
      if( part->mountpoint().compare("/") == 0 ) {
        dev_path = part->index();
        break;
      }
    }
    
    if( dev_path.empty() ) {
      LOG_E() << "no root partition found... could not mount root-partition... Abort!";
      return false;
    }
    
    Directory dir("/tmp/cloudos/installer/host_disk");
    if( dir.isWriteable() == false ) {
      return false;
    }
    
    Command mount("mount");
    mount << dev_path << dir.getDirectory().string();
    
    if( mount.waitUntilFinished() != 0 ) {
      LOG_E() << "mounting " << dev_path << " to " << dir.getDirectory().string() << " failed... Abort!";
      return false;
    }
    
    if( replicateToDirectory(dir) == false ) {
      LOG_E() << "copying files to c_disk mountpoint failed... Abort!";
      return false;
    }
    
    bool chroot_enabled = c_chroot->isEnabled();
    if( chroot_enabled && c_chroot->disable() == false ) {
      LOG_E() << "unable to disable chroot for old host tmp-dir... Abort!";
      return false;
    }
    
    std::string old_host_dir = getRootDir();
    
    // switch to our new root dir...
    if( setRootPath( dir.getDirectory() ) != 0 ) {
      LOG_E() << "switching to our new root dir failed... Abort!";
      return false;
    }
    
    c_chroot = ChRootControllerPointer( new ChRootController( dir.getDirectory().string() ) );
    if( chroot_enabled && c_chroot->enable() == false ) {
      LOG_E() << "unable to enable new chroot env... Abort!";
      return false;
    }
    
    // now clean up the old env...
    fs::remove_all( old_host_dir );
    
    return true;
  }
  
  bool InstallerHostSystem::stepLaunchManagementInstaller() {
    
    if( !c_mgt_installer ) {
      LOG_E() << "this function is called, but our management pointer is invalid... Abort!";
      return false;
    }
    
    ThreadPointer mgt( new boost::thread( boost::bind(&InstallerManagementSystem::setup, c_mgt_installer.get())) );
    LOG_I() << "management installer started with ID: " << mgt->get_id();
    c_mgt_installer_thread = mgt;
    
    return true;
  }
  
  bool InstallerHostSystem::stepFinishManagementInstaller() {
    
    if( !c_mgt_installer_thread ) {
      LOG_E() << "this function is called, but our thread pointer is invalid... Abort!";
      return false;
    }
    
    LOG_D() << "joining management installer...";
    c_mgt_installer_thread->join();
    LOG_D() << "done";
    
    fs::path mgt_image_path = c_mgt_installer->getImagePath();
    std::string mgt_image_string = mgt_image_path.string();
    LOG_D() << "management image should be located at " << mgt_image_string;
    
    if( fs::exists(mgt_image_path) == false ) {
      LOG_E() << "image file does not exist... Abort!";
      return false;
    }
    
    fs::path mgt_destination = getRootDirAsPath() / "srv/management_root.qcow2";
    LOG_I() << "copy management installer image to " << mgt_destination.string();
    
    boost::system::error_code e_copy;
    fs::copy(mgt_image_path, mgt_destination, e_copy);
    if( e_copy ) {
      LOG_E() << "copy failed! Reason: " << e_copy.message() << " Abort!";
      return false;
    }
    
    return true;
  }
  
  // 
  // P R O T E C T E D
  // 
  void InstallerHostSystem::init() {
    
    LOG_I() << "loading steps...";
    appendInstallerStep( "prepare_coredata", boost::bind(&InstallerHostSystem::stepPrepareCoreSystem,   this) );
    appendInstallerStep( "package_install",  boost::bind(&InstallerHostSystem::stepInstallHostPackages, this) );
    appendInstallerStep( "services_disable", boost::bind(&InstallerHostSystem::stepDisableServices,     this) );
    appendInstallerStep( "network_conf",     boost::bind(&InstallerHostSystem::stepConfigureNetwork,    this) );
    appendInstallerStep( "dns_conf",         boost::bind(&InstallerHostSystem::stepConfigureNameserver, this) );
    appendInstallerStep( "hostname_conf",    boost::bind(&InstallerHostSystem::stepConfigureHostname,   this) );
    appendInstallerStep( "firewall_conf",    boost::bind(&InstallerHostSystem::stepConfigureFirewall,   this) );
    appendInstallerStep( "setup_fstab",      boost::bind(&InstallerHostSystem::stepConfigureFSTab,      this) );
    appendInstallerStep( "openstack_conf",   boost::bind(&InstallerHostSystem::stepConfigureOpenStack,  this) );
    appendInstallerStep( "enable_postinst",  boost::bind(&InstallerHostSystem::stepEnablePostInstall,   this) );
    appendInstallerStep( "cleanup",          boost::bind(&InstallerHostSystem::stepCleanUpSetup,        this) );
    appendInstallerStep( "mv_data_to_disk",  boost::bind(&InstallerHostSystem::stepMoveDataToDisk,      this) );
    appendInstallerStep( "grub_conf",        boost::bind(&InstallerHostSystem::stepConfigureGrub,       this) );
    appendInstallerStep( "grub_install",     boost::bind(&InstallerHostSystem::stepInstallGrub,         this) );
    LOG_I() << "steps loaded";
    
    c_config_network.reset();
    c_config_system.reset();
    c_config_installer.reset();
    c_config_neutron.reset();
  }

  void InstallerHostSystem::checkIsValid() {
    if( isRootDirValid() == false ) {
      LOG_D() << "root_dir is invalid";
      return;
    }
    if( !c_config_network ) {
      LOG_D() << "c_config_network is invalid";
      return;
    }
    if( !c_config_system ) {
      LOG_D() << "c_config_system is invalid";
      return;
    }
    if( !c_config_installer ) {
      LOG_D() << "c_config_installer is invalid";
      return;
    }
    if( c_disk && c_disk->isValid() == false ) {
      LOG_D() << "c_disk is set, but invalid";
      return;
    }
    if( !c_config_neutron ) {
      LOG_D() << "c_config_neutron is invalid";
      return;
    }
    c_is_valid = true;
  }
  
}}  // cloudos::system
