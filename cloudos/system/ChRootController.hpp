
#ifndef CLOUDOS_SYSTEM_CHROOT_CONTROLLER_HPP__
#define CLOUDOS_SYSTEM_CHROOT_CONTROLLER_HPP__

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/process.hpp>

#include <cloudos/core/Object.hpp>
#include <cloudos/core/ShareAble.hpp>

namespace ps = boost::process;

namespace cloudos {
namespace system {
  
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
    bool setup();
    
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
    
  protected:
    
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
    
    void init();
    
  private:
    std::string c_root_path;
    
    bool c_chroot_is_active;
    
    const std::string c_chroot_cmd;
    
  };
}}
  
#endif
