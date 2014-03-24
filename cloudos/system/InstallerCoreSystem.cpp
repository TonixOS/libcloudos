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



#include <sstream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/replace.hpp>
namespace algo = boost::algorithm;

#include <cloudos/system/InstallerCoreSystem.hpp>


namespace cloudos {
namespace system {
  
  InstallerCoreSystem::InstallerCoreSystem ( const std::string& p_root_dir ) : Installer( p_root_dir ) {
    init();
  }
  
  InstallerCoreSystem::InstallerCoreSystem ( std::string && p_root_dir ) : Installer( std::move(p_root_dir) ) {
    init();
  }
  
  InstallerCoreSystem::InstallerCoreSystem ( fs::path&& p_root_dir ) : Installer ( std::move(p_root_dir) ) {
    init();
  }
  
  InstallerCoreSystem::InstallerCoreSystem ( const fs::path& p_root_dir ) : Installer ( p_root_dir ) {
    init();
  }
  
  bool InstallerCoreSystem::stepUnpackCorePackages() {
    
    if( reportInvalidObjectError( __PRETTY_FUNCTION__ ) ) {
      return false;
    }
    
    // we need to prepare our installer env
    // (via filesystem rpms)
    if( searchForRPMsInDirectory("prepare") == false ) {
      LOG_E() << "prepare RPMs are required! Abort!";
      return false;
    }
    
    if( unpackListOfPackages() == false ) {
      LOG_E() << "unpacking \"prepare\" packages faild! Abort!";
      return false;
    }
    
    std::vector<std::string> search_result;
    std::swap( search_result, c_package_list );
    
    
    // find core packages
    if( searchForRPMsInDirectory("core") == false ) {
      LOG_E() << "core RPMs are required! Abort!";
      return false;
    }
    
    if( unpackListOfPackages() == false ) {
      LOG_E() << "unpacking \"core\" packages faild! Abort!";
      return false;
    }
    
    // add "prepare" packages to our list of packages for zypper install, later on...
    for(auto& pkg : search_result) {
      c_package_list.push_back( std::move( pkg ) );
    }
    
    return true;
  }
  
  bool InstallerCoreSystem::stepInstallCorePackages() {
    return installPackages();
  }
  
  bool InstallerCoreSystem::stepCleanUpCore() {
    // some clean up
    fs::remove_all( getRootDirAsPath() / "prepare" );
    fs::remove_all( getRootDirAsPath() / "core"    );
    
    return true;
  }
  
  // 
  // P R O T E C T E D
  // 
  void InstallerCoreSystem::init() {
    // 
    // init   S T E P s
    // 
    c_package_list.clear();
    
    appendInstallerStep( "unpack_rpms",  boost::bind(&InstallerCoreSystem::stepUnpackCorePackages,  this) );
    appendInstallerStep( "set_chroot",   boost::bind(&InstallerCoreSystem::enableChroot,            this) );
    appendInstallerStep( "install_rpms", boost::bind(&InstallerCoreSystem::stepInstallCorePackages, this) );
    appendInstallerStep( "unset_chroot", boost::bind(&InstallerCoreSystem::disableChroot,           this) );
    appendInstallerStep( "cleanup_core", boost::bind(&InstallerCoreSystem::stepCleanUpCore,         this) );
  }
  
}} // cloudos::system
