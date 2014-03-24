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



#ifndef CLOUDOS_SYSTEM_CHROOT_CONTROLLER_HPP__
#define CLOUDOS_SYSTEM_CHROOT_CONTROLLER_HPP__

#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/process.hpp>

#include <cloudos/core/Object.hpp>
#include <cloudos/core/ShareAble.hpp>

namespace ps = boost::process;

namespace cloudos {
namespace system {

  class Command; // forward declaration
  
  class ChRootController;
  typedef boost::shared_ptr<ChRootController> ChRootControllerPointer;
  
  /**
   * Why we need this class?
   * You will be able to control an ChRoot environment. We need it within a class,
   * as this provides all needed information about the chroot in one object.
   * And you will be able to copy the ChRoot controlling.
   * 
   * What is possible with this class?
   * Set up a chroot environment. If you like to execute a Command within this object,
   * give the ChRoot object to the command object.
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * ChRootController chroot(/path/to/chroot/);
   * chroot.setup();
   * if chroot.isValid():
   *   chroot.getRootPath();
   * 
   * TODO: implement umount after failure in mount... use a vector of umount commands and IndexName
   *       of the mount command; then connect "Command::sigFinished()" with a function, that
   *       umounts all needed values
   * TODO: make multi-theading able
   */
  class ChRootController : public core::Object, public core::ShareAble {
  public:
    ChRootController( std::string&& p_root_path );
    ChRootController(const std::string& p_root_path);
    ~ChRootController();
    
    /**
     * Will set up the chroot environment.
     *   - mount tmpfs/devfs/sysfs etc.
     */
    bool enable();
    
    /**
     * Will unmount all chroot related mountpoints.
     * If chroot isn't set, it will return true anyway
     */
    bool disable();
    
    /**
     * Returns true, if chroot is currently enabled
     */
    bool isEnabled() { return c_chroot_is_active; }
    
    /**
     * returns the path to the chroot environment.
     */
    const std::string& getRootPath() const;
    
    /**
     * returns the path+filename of the chroot binary
     */
    const std::string& getChrootBin() const {
      return c_chroot_cmd;
    }
    
    /**
     * sets the path to the chroot environment
     * Returns true, if the path is valid
     */
    bool setRootPath(std::string&& p_root_path);
    
    /**
     * our "old style" version of setRootPath
     */
    inline bool setRootPath(const std::string& p_root_path) {
      return setRootPath(std::move(p_root_path));
    }
    
  protected:
    
    void init();
    
  private:
    std::string c_root_path;
    
    bool c_chroot_is_active;
    
    std::string c_chroot_cmd;
    
    /**
     * Will retry to execute the umount/mount command, until it might succeed.
     * Sometimes, umount/mount fails a few times... we will retry it for a short time
     * to increase the possibility to succeed.
     * If it faild all times, we will abort.
     * 
     * Returns 0, if command succeeded; else 1
     * This way, we are able to combine multiple umounts and check, if all ended successful.
     */
    uint retryUntilSucceeded( Command& p_command );
    
  };
}} // cloudos::system
  
#endif
