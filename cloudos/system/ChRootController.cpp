
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <cloudos/system/ChRootController.hpp>
#include <cloudos/system/Command.hpp>

#include <cloudos/core/command_definitions.hpp>

namespace cloudos {
namespace system {
  
  ChRootController::ChRootController ( std::string && p_root_path ) : c_chroot_cmd( G_COMMAND_CHROOT ) {
    init();
    setRootPath( std::move(p_root_path) );
  }
  
  ChRootController::ChRootController ( const std::string& p_root_path ) : c_chroot_cmd( G_COMMAND_CHROOT ) {
    init();
    setRootPath( std::move(p_root_path) );
  }
  
  ChRootController::~ChRootController() {
    if( c_chroot_is_active == false ) {
      return;
    }
    const std::string umount_cmd( G_COMMAND_UMOUNT );
    Command umount_shm( umount_cmd );
    umount_shm << "-v" << c_root_path + "/dev/shm";
    umount_shm.start();
    
    Command umount_devpts( umount_cmd );
    umount_devpts << "-v" << c_root_path + "/dev/pts";
    umount_devpts.start();
    
    Command umount_sysfs( umount_cmd );
    umount_sysfs << "-v" << c_root_path + "/sys";
    umount_sysfs.start();
    
    Command umount_proc( umount_cmd );
    umount_proc << "-v" << c_root_path + "/proc";
    umount_proc.start();
    
    // wait until all required dependencies are umounted
    // TODO: check for errors and report them!
    umount_shm.waitUntilFinished();
    umount_devpts.waitUntilFinished();
    
    Command umount_dev( umount_cmd );
    umount_dev << "-v" << c_root_path + "/dev";
    umount_dev.start();
    
    // TODO: check for errors and report them
    umount_sysfs.waitUntilFinished();
    umount_proc.waitUntilFinished();
    umount_dev.waitUntilFinished();
  }
  
  bool ChRootController::setup() {
    if( c_chroot_is_active ) {
      return true;
    }
    if( c_is_valid == false ) {
      // TODO: LOG error
      return false;
    }
    
    // bind devfs
    const std::string cmd( G_COMMAND_MOUNT );
    Command mount_dev( cmd );
    mount_dev << "-v" << "--bind" << "/dev" << c_root_path + "/dev";
    mount_dev.start();
    
    // mount devpts
    Command mount_devpts( cmd );
    mount_devpts << "-vt" << "devpts" << "devpts" << c_root_path + "/dev/pts";
    mount_devpts.start();
    
    // mount proc
    Command mount_proc( cmd );
    mount_proc << "-vt" << "proc" << "proc" << c_root_path + "/proc";
    mount_proc.start();
    
    // mount sysfs
    Command mount_sysfs( cmd );
    mount_sysfs << "-vt" << "sysfs" << "sysfs" << c_root_path + "/sys";
    mount_sysfs.start();
    
    // check, if /dev/shm is already a directory
    fs::path shm(c_root_path);
    shm = shm / "dev" / "shm";
    if( fs::exists( shm ) == false ) {
      fs::create_directory( shm );
    }
    
    // mount shared memory
    Command mount_shm( cmd );
    mount_shm << "-vt" << "tmpfs" << "shm" << c_root_path + "/dev/shm";
    mount_shm.start();
    
    // 
    // Now check the results
    // TODO: report error in LOG
    if( mount_dev.waitUntilFinished()    != 0 ) {
      return false;
    }
    if( mount_devpts.waitUntilFinished() != 0 ) {
      return false;
    }
    if( mount_proc.waitUntilFinished()   != 0 ) {
      return false;
    }
    if( mount_sysfs.waitUntilFinished()  != 0 ) {
      return false;
    }
    if( mount_shm.waitUntilFinished()    != 0 ) {
      return false;
    }
    c_chroot_is_active = true;
    return true;
  }
  
  const std::string& ChRootController::getRootPath() const {
    return c_root_path;
  }

  //  
  //  P R O T E C T E D
  //  
  
  bool ChRootController::setRootPath ( std::string && p_root_path ) {
    fs::path root( std::move(p_root_path) );
    if( fs::exists( root ) == false || fs::is_directory( root ) == false ) {
      c_is_valid = false;
      return false;
    }
    c_root_path = std::move( fs::absolute( std::move(root) ).string() );
    
    c_is_valid = true;
    return true;
  }

  void ChRootController::init() {
    c_chroot_is_active = false;
  }
  
}}
