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



// for usleep()
extern "C" {
#include <unistd.h>
}

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <cloudos/system/ChRootController.hpp>
#include <cloudos/system/Command.hpp>
#include <cloudos/system/Directory.hpp>

namespace cloudos {
namespace system {
  
  ChRootController::ChRootController ( std::string && p_root_path ) : c_chroot_cmd( "chroot" ) {
    init();
    setRootPath( std::move(p_root_path) );
    Command::toBinFromPATH( c_chroot_cmd );
  }
  
  ChRootController::ChRootController ( const std::string& p_root_path ) : c_chroot_cmd( "chroot" ) {
    init();
    std::string root_path = p_root_path;
    setRootPath( std::move(root_path) );
    Command::toBinFromPATH( c_chroot_cmd );
  }
  
  ChRootController::~ChRootController() {
    disable();
  }
  
  bool ChRootController::enable() {
    if( c_chroot_is_active ) {
      return true;
    }
    if( reportInvalidObjectError( __PRETTY_FUNCTION__ ) ) {
      return false;
    }
    
    LOG_D() << "try to enable chroot";
    
    // bind devfs
    Command mount_dev;
    mount_dev.setIndexName("/dev");
    mount_dev << "mount" << "-v" << "--bind" << "/dev" << c_root_path + "/dev";
    mount_dev.start();
    
    // mount proc
    Command mount_proc;
    mount_proc.setIndexName("/proc");
    mount_proc << "mount" << "-vt" << "proc" << "proc" << c_root_path + "/proc";
    mount_proc.start();
    
    // mount sysfs
    Command mount_sysfs;
    mount_sysfs.setIndexName("/sys");
    mount_sysfs << "mount" << "-vt" << "sysfs" << "sysfs" << c_root_path + "/sys";
    mount_sysfs.start();
    
    // 
    // Now check the results
    // FIXME: unmount stack backwards, if mounting a fs failed... so we leaf the system in a clean state...
    if( mount_dev.waitUntilFinished()   != 0 ) {
      LOG_E() << "mounting chroot dev failed!";
      return false;
    }
    if( mount_proc.waitUntilFinished()  != 0 ) {
      LOG_E() << "mounting chroot proc failed!";
      return false;
    }
    if( mount_sysfs.waitUntilFinished() != 0 ) {
      LOG_E() << "mounting chroot sys failed!";
      return false;
    }
    
    // check, if /dev/shm is already a directory
    fs::path shm(c_root_path);
    shm = shm / "dev" / "shm";
    if( fs::exists( shm ) == false ) {
      fs::create_directory( shm );
    }
    Directory shm_dir( c_root_path + "/dev/shm" );
    if( shm_dir.createIfNotExists() != 0 ) {
      LOG_E() << "need " << c_root_path << "/dev/shm directory for chroot! None available! Abort!";
      //FIXME undo mounting...
      return false;
    }
    
    // mount shared memory
    Command mount_shm;
    mount_shm.setIndexName("/dev/shm");
    mount_shm << "mount" << "-vt" << "tmpfs" << "shm" << c_root_path + "/dev/shm";
    mount_shm.start();
    
    // mount devpts
    Command mount_devpts;
    mount_devpts.setIndexName("/dev/pts");
    mount_devpts << "mount" << "-vt" << "devpts" << "devpts" << c_root_path + "/dev/pts";
    mount_devpts.start();
    
    if( mount_shm.waitUntilFinished()    != 0 ) {
      LOG_E() << "mounting chroot shm failed!";
      return false;
    }
    if( mount_devpts.waitUntilFinished() != 0 ) {
      LOG_E() << "mounting chroot dev/pts failed!";
      return false;
    }
    
    LOG_D() << "enable chroot succeed";
    c_chroot_is_active = true;
    return true;
  }
  
  bool ChRootController::disable() {
    if( c_chroot_is_active == false ) {
      return true;
    }
    
    LOG_D() << "try to disable chroot";
    
    Command umount_shm;
    umount_shm.setIndexName("/dev/shm");
    umount_shm << "umount" << "-v" << c_root_path + "/dev/shm";
    umount_shm.start();
    
    Command umount_devpts;
    umount_devpts.setIndexName("/dev/pts");
    umount_devpts << "umount" << "-v" << c_root_path + "/dev/pts";
    umount_devpts.start();
    
    Command umount_sysfs;
    umount_sysfs.setIndexName("/sys");
    umount_sysfs << "umount" << "-v" << c_root_path + "/sys";
    umount_sysfs.start();
    
    Command umount_proc;
    umount_proc.setIndexName("/proc");
    umount_proc << "umount" << "-v" << c_root_path + "/proc";
    umount_proc.start();
    
    // wait until all required dependencies are umounted
    uint all_ok = retryUntilSucceeded( umount_shm ) +
                  retryUntilSucceeded( umount_devpts );
    
    Command umount_dev;
    umount_dev.setIndexName("/dev");
    umount_dev << "umount" << "-v" << c_root_path + "/dev";
    umount_dev.start();
    
    // wait until umount is done...
    all_ok += retryUntilSucceeded( umount_sysfs ) +
              retryUntilSucceeded( umount_proc )  +
              retryUntilSucceeded( umount_dev );
    
    LOG_D() << "disable chroot succeed";
    
    c_chroot_is_active = false;
    return all_ok == 0;
  }
  
  const std::string& ChRootController::getRootPath() const {
    return c_root_path;
  }

  //  
  //  P R O T E C T E D
  //  
  
  bool ChRootController::setRootPath ( std::string && p_root_path ) {
    LOG_I() << "setting chroot base: " << p_root_path;
    
    Directory chroot_base( std::move(p_root_path) );
    if( chroot_base.isAccessible() == false ) {
      return false;
    }
    c_root_path = std::move( chroot_base.getDirectory().string() );
    
    LOG_I() << c_root_path << " as chroot base set";
    c_is_valid = true;
    return true;
  }

  void ChRootController::init() {
    c_chroot_is_active = false;
  }
  
  // 
  // P R I V A T E
  // 
  uint ChRootController::retryUntilSucceeded ( Command& p_command ) {
    int i = 0;
    for( ; i != 10 && p_command.waitUntilFinished() != 0; ++i ) {
      LOG_W() << "umount failed for " << p_command.getIndexName() << "retry";
      usleep(20000); // wait micro seconds ( 20ms )
      p_command.start(); // start command again
    }
    
    if( i == 10 ) {
      LOG_E() << "umount for " << p_command.getIndexName() << " FAILED! Abort!";
      c_is_valid = false;
      return 1;
    }
    
    return 0;
  }

  
}}
