
#ifndef CLOUDOS_SYSTEM_INSTALLER_CORE_SYSTEM_HPP__
#define CLOUDOS_SYSTEM_INSTALLER_CORE_SYSTEM_HPP__

#include <string>
#include <vector>

#include <boost/function.hpp>
#include <boost/container/stable_vector.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <cloudos/core/Object.hpp>
#include <cloudos/system/ChRootController.hpp>
#include <cloudos/system/Command.hpp>
#include <cloudos/system/Directory.hpp>
#include <cloudos/system/File.hpp>
#include <cloudos/system/FileTar.hpp>
#include <cloudos/system/Installer.hpp>

namespace cloudos {
namespace system {
  
  class InstallerCoreSystem;
  typedef boost::shared_ptr<InstallerCoreSystem> InstallerCoreSystemPointer;
  
  /**
   * Why we need this class?
   * This class is able to install a core system
   * 
   * What is possible with this class?
   * Create a base system
   * 
   * Is there a special behaviour, on special circumstances?
   * Non known
   * 
   * How to use this class as an object?
   * InstallerCoreSystem coresys(/tmp/path/to/install/);
   * coresys.setup();
   */
  class InstallerCoreSystem : public Installer {
  public:
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerCoreSystem(const std::string& p_root_dir);
    
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerCoreSystem(std::string&& p_root_dir);
    
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerCoreSystem(fs::path&& p_root_dir);
    
    /**
     * Every CoreSystem needs at least a base path, where it will be installed in.
     * This directory will be givven by p_root_dir
     */
    InstallerCoreSystem(const fs::path& p_root_dir);
    
    /**
     * Will unpack all RPMs into the base path (c_root_dir) 
     */
    bool stepUnpackCorePackages();
    
    /**
     * Calls zypper to install our core packages
     */
    bool stepInstallCorePackages();
    
    /**
     * Cleans up the installation...
     * 
     * e.g. removes unwanted files
     */
    bool stepCleanUpCore();
    
  protected:
    
    /**
     * Inits all class members...
     */
    virtual void init() override;
    
  private:
  };
}}

#endif
