
#ifndef CLOUDOS_SYSTEM_INSTALLER_HPP__
#define CLOUDOS_SYSTEM_INSTALLER_HPP__

#include <string>
#include <vector>

#include <boost/function.hpp>
#include <boost/container/stable_vector.hpp>

#include <cloudos/core/Object.hpp>
#include <cloudos/system/ChRootController.hpp>
#include <cloudos/system/Command.hpp>
#include <cloudos/system/Directory.hpp>
#include <cloudos/system/File.hpp>
#include <cloudos/system/FileTar.hpp>
#include <cloudos/system/StepExecutor.hpp>

namespace cloudos {
namespace system {
  
  /**
   * Why we need this class?
   * This class is able to help setting up a system
   * 
   * What is possible with this class?
   * Create a base system and extend it...
   * 
   * Is there a special behaviour, on special circumstances?
   * Non known
   * 
   * How to use this class as an object?
   * There is no way to instanciate this class as a standalone-object.
   * Inherint it as base class!
   */
  class Installer : public core::Object {
  public:
    /**
     * Will copy all required files from *this to p_to_dir.
     * This way, you are able to copy the data and build a second installation base.
     */
    virtual bool replicateToDirectory( Directory& p_to_dir ) const;
    
    
    /**
     * Sets up all needed mountpoints for a successful chroot env.
     */
    bool enableChroot();
    
    /**
     * Will unmount all chroot related mountpoints
     */
    bool disableChroot();
    
    /**
     * Set the tar file, which includes all core rpm packages.
     * We need this file to be this->isValid() == true
     */
    virtual bool setPackagesTarFile( fs::path&& p_tar_file );
    
    /**
     * Returns the root dir string from this running object.
     */
    std::string getRootDir() const;
    
    /**
     * Returns the root path object from this running object.
     */
    fs::path getRootDirAsPath() const;
    
    /**
     * Will check and set the root_path for the installation.
     * 
     * Returns >0, if setting the root path failed.
     *   1 => could not create directory (will try it, if it not exists jet)
     *   4 => p_root_path is not a directory
     *   6 => not enough rights to work with the directory
     * Other return values are currently not in use...
     */
    ushort setRootPath( std::string&& p_root_path );
    
    /**
     * Will check and set the root_path for the installation.
     * 
     * Returns >0, if setting the root path failed.
     *   1 => could not create directory (will try it, if it not exists jet)
     *   4 => p_root_path is not a directory
     *   6 => not enough rights to work with the directory
     * Other return values are currently not in use...
     */
    ushort setRootPath( fs::path&& p_root_path );
    
    /**
     * Will check and set the root_path for the installation.
     * 
     * Returns >0, if setting the root path failed.
     *   1 => could not create directory (will try it, if it not exists jet)
     *   4 => p_root_path is not a directory
     *   6 => not enough rights to work with the directory
     * Other return values are currently not in use...
     */
    ushort setRootPath( const fs::path& p_root_path );
    
    /**
     * Will set up a core system. Will execute all available steps, provided in c_steps.
     * You migth want to add steps via addInstallerStep( InstallerStepVector::iterator p_after )
     * 
     * TODO: check, if step is already done...
     */
    virtual bool setup();
    
    /**
     * if setup was already called.
     */
    bool isSetupFinished();
    
  protected:
    Installer() {}
    
    /**
     * To control our chroot env.
     */
    ChRootControllerPointer c_chroot;
    
    /**
     * All package (RPM) names, required by a base core installation.
     * 
     * You are able to add packages from a subclass and those will be installed
     * by our step "stepInstallCorePackages".
     * Just add packages, before calling setup().
     */
    std::vector<std::string> c_package_list;
    
    /**
     * Disable direct object instanciation
     */
    Installer( const std::string& p_root_dir );
    Installer( std::string && p_root_dir );
    Installer( const fs::path& p_root_dir );
    Installer( fs::path&& p_root_dir );
    
    /**
     * Will insert p_step to c_steps after element p_after.
     * This way, you are able to extend the install process.
     */
    void addInstallerStep( StepExecutor&& p_step, InstallerStepVector::const_iterator& p_after );
    
    /**
     * Inserts a installer step, after the given p_after. If none is found,
     * it will insert the step at the end.
     */
    void insertInstallerStep(std::string&& p_index_name, boost::function<bool ()>&& p_callback, std::string&& p_after);
    
    /**
     * Will add p_step to the end of c_steps.
     * For more information, see addInstallerStep(...)
     */
    void appendInstallerStep( StepExecutor&& p_step );
    
    /**
     * Creates a new installer step with the given arguments...
     */
    void appendInstallerStep( std::string&& p_index_name, boost::function<bool ()>&& p_callback );
    
    /**
     * Will push the step at the front of c_steps
     */
    void pushFrontInstallerStep( std::string&& p_index_name, boost::function<bool ()>&& p_callback );
    
    /**
     * Removes the first step, that has p_index_name.
     */
    void removeInstallerStep( std::string&& p_index_name );
    
    /**
     * Will return a const set (stable vector) of all steps, which are available.
     * This way, you are able to find the right pos for addInstallerStep()
     */
    const InstallerStepVector& getSteps();
    
    /**
     * Calls zypper to install the list of packages in c_package_list.
     * Will remove leading c_root_dir package path prefix.
     */
    bool installPackages() const;
    
    /**
     * Inits all class members...
     */
    virtual void init();
    
    /**
     * Will check, if all requirements are met and we are able to perform our task...
     */
    virtual void checkIsValid() override;
    
    /**
     * Searches for a directory given by pattern (just the dir name, without path!)
     * and searches for *.rpm within this directory (recursive search for up to 3 level).
     * 
     * The result will be stored in c_package_list
     * 
     * Returns true, if RPMs are found
     */
    bool searchForRPMsInDirectory( std::string&& p_pattern );
    
    /**
     * Unpacks the packages stored in c_package_list via shell script
     */
    bool unpackListOfPackages();
    
    /**
     * returns the value of c_root_dir.isValid()
     */
    bool isRootDirValid() const {
      return c_root_dir.isValid();
    }
    
    /**
     * Get access to our FileTar object
     */
    FileTarPointer getFileTar() {
      return c_rpm_packages_file;
    }
    
  private:
    
    /**
     * Where our system will be installed...
     */
    Directory c_root_dir;
    
    /**
     * Ordered and available installer steps, which need to be done, to set up a complete system.
     */
    InstallerStepVector c_steps;
    
    /**
     * Indicates, if the setup() is called and was successful.
     */
    bool c_setup_finished;
    
    /**
     * This file, which includes our core rpm packages
     */
    FileTarPointer c_rpm_packages_file;
    
  };
  
}} // cloudos;;system

#endif
