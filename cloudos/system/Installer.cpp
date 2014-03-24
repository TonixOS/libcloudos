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




// for access() call in setRootPath()
extern "C" {
#include <unistd.h>
}

#include <boost/range/adaptor/reversed.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/replace.hpp>
namespace algo = boost::algorithm;

#include <cloudos/system/Installer.hpp>

namespace cloudos {
namespace system {
  
  bool Installer::replicateToDirectory ( Directory& p_to_dir ) const {
    LOG_I() << "replicate setup (" << c_root_dir.getDirectory().string() << ") to " << p_to_dir.getDirectory();
    
    if( p_to_dir.isWriteable() == false ) {
      LOG_E() << "faild, as destination is not writeable! Abort";
      return false;
    }
    
    bool chroot_state = c_chroot->isEnabled();
    if( chroot_state ) {
      c_chroot->disable();
    }
    
    Command copy;
    copy << "cp" << "-a" << c_root_dir.getDirectory().string() + "/." << p_to_dir.getDirectory().string();
    
    if( copy.waitUntilFinished() != 0 ) {
      LOG_E() << "copying files FAILD!";
      return false;
    }
    
    LOG_I() << "SUCCESSFUL";
    
    if( chroot_state ) {
      c_chroot->enable();
    }
    return true;
  }
  
  bool Installer::enableChroot() {
    bool enabled = c_chroot->enable();
    if(enabled == false) {
      return enabled;
    }
    
    // test chroot env
    Command root;
    root << "stat" << "-c" << "%d:%i" << "/";
    root.waitUntilFinished();
    
    Command chroot_test;
    chroot_test.setChRootController( c_chroot );
    chroot_test << "stat" << "-c" << "%d:%i" << "/";
    
    if( chroot_test.waitUntilFinished() != 0 ) {
      LOG_E() << "chroot test failed!";
      return false;
    }
    // check for a different inode number
    return root.getStdoutOutput().str() != chroot_test.getStdoutOutput().str();
  }
  
  bool Installer::disableChroot() {
    return c_chroot->disable();
  }
  
  bool Installer::setPackagesTarFile ( fs::path && p_tar_file ) {
    LOG_T() << "set tar file: " << p_tar_file.string();
    Directory copy_dir( std::move(c_root_dir.getDirectory()) );
    c_rpm_packages_file = FileTarPointer( new FileTar(std::move(p_tar_file), std::move(copy_dir)) );
    
    if( c_rpm_packages_file->isValid() == false ) {
      LOG_E() << "tar file is invalid... Abort!";
      return false;
    }
    
    // unpack tar file
    if( c_rpm_packages_file->unpack() != 0 ) {
      LOG_E() << "unable to unpack RPM packages tar file! Abort!";
      c_is_valid = false;
      return false;
    }
    
    checkIsValid();
    return true;
  }
  
  std::string Installer::getRootDir() const {
    std::string root( c_root_dir.getDirectory().string() );
    return root;
  }
  
  fs::path Installer::getRootDirAsPath() const {
    return c_root_dir.getDirectory();
  }
  
  ushort Installer::setRootPath ( std::string && p_root_path ) {
    fs::path root( p_root_path );
    return setRootPath( std::move(root) );
  }
  
  ushort Installer::setRootPath ( fs::path&& p_root_path ) {
    LOG_I() << "init installer for root_path " << p_root_path;
    // 
    // ensure, that our root path exists; create it, if necessary
    // 
    c_root_dir = std::move(Directory( p_root_path ));
    if( c_root_dir.isWriteable() == false ) {
      LOG_E() << "direcotry needs to be write- and accessible; but it isn't! Abort!";
      return 6;
    }
    
    checkIsValid(); // check, if our object is now save to be used...
    return 0;
  }
  
  ushort Installer::setRootPath ( const fs::path& p_root_path ) {
    LOG_I() << "init installer for root_path " << p_root_path;
    // 
    // ensure, that our root path exists; create it, if necessary
    // 
    c_root_dir = std::move(Directory( p_root_path ));
    if( c_root_dir.isWriteable() == false ) {
      LOG_E() << "direcotry needs to be write- and accessible; but it isn't! Abort!";
      return 6;
    }
    
    checkIsValid(); // check, if our object is now save to be used...
    return 0;
  }
  
  bool Installer::setup() {
    
    if( c_setup_finished ) {
      LOG_D() << "we're already done... nothing to do... return";
      return true;
    }
    
    if( reportInvalidObjectError( __PRETTY_FUNCTION__ ) ) {
      return false;
    }
    
    // don't check for validation, as we might call external functions as well...
    // so each function needs to check validation for itself!
    
    for( auto& step : c_steps ) {
      LOG_D() << "running step " << step.getIndexName();
      
      //FIXME emit signal for success and error after each finished step; with details about the step
      if( step.run() == false ) {
        LOG_E() << "step " << step.getIndexName() << " FAILD!";
        return false;
      }
      
      LOG_I() << "step " << step.getIndexName() << " finished successful!";
    }
    
    c_setup_finished = true;
    return true;
  }
  
  bool Installer::isSetupFinished() {
    return c_setup_finished;
  }
  
  
  // 
  // P R O T E C T E D
  // 
  
  Installer::Installer ( const std::string& p_root_dir ) {
    std::string copy( p_root_dir );
    setRootPath( std::move(copy) );
    init();
  }
  
  Installer::Installer ( std::string && p_root_dir ) {
    setRootPath( std::move(p_root_dir) );
    init();
  }
  
  Installer::Installer ( const fs::path& p_root_dir ) {
    setRootPath( p_root_dir );
    init();
  }
  
  Installer::Installer ( fs::path && p_root_dir ) {
    setRootPath( std::move(p_root_dir) );
    init();
  }
  
  void Installer::addInstallerStep ( StepExecutor&& p_step, InstallerStepVector::const_iterator& p_after ) {
    LOG_D() << "insert step " << p_step.getIndexName() << " after step " << p_after->getIndexName();
    c_steps.insert( p_after, std::move( p_step ) );
  }
  
  void Installer::insertInstallerStep (std::string&& p_index_name, boost::function<bool ()>&& p_callback, std::string&& p_after ) {
    LOG_D() << "insert step " << p_index_name << " after " << p_after;
    
    StepExecutor step;
    step.setIndexName( std::move(p_index_name) );
    step.setCallback(  std::move(p_callback)   );
    
    InstallerStepVector::const_iterator element = c_steps.cbegin(), end = c_steps.cend();
    while(element != end && element->getIndexName() != p_after) {
      ++element;
    }
    
    if( element != end ) {
      LOG_D() << p_after << " found... inserting after it...";
    }
    
    c_steps.insert(element, std::move(step));
    return;
  }
  
  void Installer::appendInstallerStep ( StepExecutor && p_step ) {
    LOG_D() << "appending step " << p_step.getIndexName();
    c_steps.push_back( std::move(p_step) );
  }
  
  void Installer::appendInstallerStep ( std::string && p_index_name, boost::function<bool ()>&& p_callback ) {
    LOG_D() << "appending step " << p_index_name;
    StepExecutor step;
    step.setIndexName( std::move(p_index_name) );
    step.setCallback(  std::move(p_callback)   );
    
    c_steps.push_back( std::move(step) );
  }
  
  void Installer::pushFrontInstallerStep ( std::string && p_index_name, boost::function<bool ()> && p_callback ) {
    LOG_D() << "prepending step " << p_index_name;
    StepExecutor step;
    step.setIndexName( std::move(p_index_name) );
    step.setCallback(  std::move(p_callback)   );
    
    c_steps.insert( c_steps.cbegin(), std::move(step) );
  }
  
  void Installer::removeInstallerStep ( std::string && p_index_name ) {
    InstallerStepVector::const_iterator it, end;
    for( it = c_steps.cbegin(), end = c_steps.cend(); it != end; ++it ) {
      if( it->getIndexName() == p_index_name ) {
        c_steps.erase( it );
        return;
      }
    }
  }
    
  const InstallerStepVector& Installer::getSteps() {
    return c_steps;
  }
  
  bool Installer::installPackages() const {
    Command cmd_zypper;
    cmd_zypper.setEnvironmentVar("INSTALL_MODE", "livecd");
    if( c_chroot->isEnabled() == false && c_chroot->enable() == false ) {
      LOG_E() << "chroot could not be enabled! Abort!";
      return false;
    }
    cmd_zypper.setChRootController( c_chroot );
    cmd_zypper << "zypper" << "--non-interactive" << "install";
    
    // now add all packages
    const int root_dir_length = getRootDir().length();
    for( auto& pkg : c_package_list ) {
      // remove leading c_root_dir as we work inside our chroot
      cmd_zypper.addArgument( pkg.substr( root_dir_length ) );
    }
    
    if( cmd_zypper.waitUntilFinished() != 0 ) {
      LOG_E() << "installing packages FAILD!";
      return false;
    }
    return true;
  }
  
  void Installer::init() {
    c_setup_finished = false;
    c_steps.clear();
    
    // 
    // prepare chroot
    // 
    c_chroot = ChRootControllerPointer( new ChRootController( std::move(getRootDir()) ) );
  }
  
  void Installer::checkIsValid() {
    c_is_valid = c_rpm_packages_file && c_rpm_packages_file->isUnpacked() && c_root_dir.isValid();
  }
  
  bool Installer::searchForRPMsInDirectory ( std::string && p_pattern ) {
    
    // 
    // Look up p_pattern
    // 
    std::vector<fs::path> search_result;
    if( c_root_dir.findFiles( search_result, p_pattern,
                              LOOKUP_BY_FULLMATCH, FILE_TYPE_DIRECTORY )  == false ) {
      LOG_E() << "no \"" << p_pattern << "\" directory found in " << c_root_dir.getDirectory();
      c_root_dir.findFiles( search_result, ".*", LOOKUP_BY_REGEX, FILE_TYPE_DIRECTORY );
    
      LOG_E() << "directories available:";
      for(auto& dir : search_result) {
        LOG_E() << dir.string();
      }
      LOG_E() << "Abort!";
      return false;
    }
    
    // there should be only one directory, called "core", as we just unpacked the rpms dir, and nothing more...
    if( search_result.size() > 1 ) {
      LOG_W() << "found more than one directory \"" << p_pattern << "\" in our install path... sounds bad... results are:";
      for(auto& dir : search_result) {
        LOG_W() << dir.string();
      }
      LOG_W() << "will go forward, but just using the first search entry!";
    }
    
    // 
    // Look up RPMs
    // 
        
    Directory rpm_dir( search_result.at(0) );
    search_result.clear();
    
    if( rpm_dir.isAccessible() == false ) {
      LOG_E() << "core dir is not accessible... Abort!";
      return false;
    }
        
    if( rpm_dir.findFiles( search_result, ".rpm", LOOKUP_BY_ENDSWITH,
                           FILE_TYPE_REGULAR | FILE_TYPE_SYMLINK, 3 ) == false ) {
      LOG_W() << "no RPMs found in " << rpm_dir.getDirectory() << " Abort!";
      return false;
    }
    
    // we need to transform the datatype
    // read the search_result in reverse order, as mostly, reading entries from a directory
    // is in DESC order, but we need ASC order
    std::vector<std::string> rpms;
    for(auto& rpm : boost::adaptors::reverse(search_result)) {
      c_package_list.push_back( std::move( rpm.string() ) );
    }
                
    // just to be sure, that we return it ASC
    // return RPMs in sorted order
    std::sort( c_package_list.begin(), c_package_list.end() );
    
    return true;
  }
  
  bool Installer::unpackListOfPackages() {
    
    std::string pkg_list = std::move( algo::join(c_package_list, " ") );
    LOG_I() << "unpack rpms..." << pkg_list;
    
    // Bash rpm2cpio script
    std::stringstream script;
    std::string root_dir( c_root_dir.getDirectory().string() );
    script << "PKGS=\"" << pkg_list << "\";"
    << "cd " << root_dir << ";"
    << "for i in $PKGS; do rpm2cpio $i | cpio -idvm; done";
    //FIXME: use ps::launch_pipeline() instead of bash
    //TODO: check for timeout...
    
    Command bash;
    bash.setOpenCommandStreams( 0 ); // disable any output, as it would be to much...
    bash << "bash" << "-xc" << script.str();
    if( bash.waitUntilFinished() != 0 ) {
      return false;
    }
    
    return true;
  }
  
}} // cloudos::system
