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



#ifndef CLOUDOS_CORE_CONFIG_VECTOR_HPP__
#define CLOUDOS_CORE_CONFIG_VECTOR_HPP__

#include <sstream>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/container/stable_vector.hpp>

#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <cloudos/core/Object.hpp>
#include <cloudos/system/File.hpp>

namespace bc = boost::container;
namespace fs = boost::filesystem;
namespace pb = google::protobuf;

namespace cloudos {
namespace core {
  
  /**
   * Why we need this class?
   * You will need this class, if you like to have a nice and simple config
   * settings (google::protobuf::Message) handling for your object. It will store/manage
   * all configs in a stable_vector.
   * 
   * What is possible with this class?
   * Load/get/set google::protobuf message objects as config sources
   * 
   * Is there a special behaviour, on special circumstances?
   *   - Header only
   *   - you are not allowed to instanciate this class directly!
   * 
   * How to use this class as an object?
   * class blubb : public ConfigVector {
   *  // some implementation here...
   * };
   */
  template<typename T>
  class ConfigVector : public Object {
  public:
    
    typedef boost::shared_ptr<T> ConfigPointer;
    typedef boost::container::stable_vector< boost::shared_ptr<T> > ConfigPointerVector;
    
    /**
     * Will load the config settings from the given path/file and add it
     * to our vector of configs.
     */
    virtual bool loadAdditionalConfig(fs::path&& p_config_path) {
      system::File config_file( std::move(p_config_path) );
      
      LOG_I() << "loading config file " << config_file.getFileNameAsPath();
      
      std::stringstream ss;
      if( config_file.getContent(ss) == false ) {
        LOG_E() << "could not read the config file " << config_file.getFileNameAsPath() << " Abort!";
        return false;
      }
      
      ConfigPointer config( new T() );
      if( pb::TextFormat::ParseFromString(ss.str(), config.get()) == false ) {
        LOG_E() << "parsing config file FAILED! Abort!";
        return false;
      }
      
      c_configs.push_back( config );
      LOG_I() << "SUCCESS";
      return true;
    }
    
    /**
     * Loads a vector of config files and adds them to our c_configs
     * member.
     * Will return false, if any file could not be loaded.
     */
    virtual bool loadAdditionalConfigs( bc::stable_vector<fs::path>&& p_config_paths ) {
      for(auto config : p_config_paths) {
        if( loadAdditionalConfig( std::move(config) ) == false ) {
          return false;
        }
      }
      return true;
    }
    
    /**
     * Adds a config object to our vector of configs
     */
    virtual bool addConfig(ConfigPointer p_config) {
      LOG_I() << "adding config data...";
      if( !p_config ) {
        LOG_E() << "given config data is invalid (nullptr). Abort!";
        return false;
      }
      c_configs.push_back(p_config);
      return true;
    }
    
    /**
     * Adds multiple config objects...
     */
    virtual void setConfigs( ConfigPointerVector&& p_configs ) {
      c_configs = p_configs;
    }
    
    /**
     * Will return a copy of the configuration
     */
    virtual ConfigPointerVector getConfigs() const {
      ConfigPointerVector copy;
      for( auto& config : c_configs ) {
        ConfigPointer conf_copy( new T() );
        conf_copy->CopyFrom( *(config.get()) );
        copy.push_back( conf_copy );
      }
      return copy;
    }
    
    /**
     * Will return a copy of the configuration matching the index
     * or an invalid pointer
     */
    virtual ConfigPointer getConfig(const std::string& p_index) const {
      for( auto& config : c_configs ) {
        if( config->index() == p_index ) {
          ConfigPointer copy( new T() );
          copy->CopyFrom( *(config.get()) );
          return copy;
        }
      }
      return ConfigPointer();
    }
    
    /**
     * Is using config->index() and compares p_index to it.
     * If it matches, it will return the iterator to the corresponding element,
     * else, it will return c_config.end()
     */
    virtual typename ConfigPointerVector::iterator findConfigByIndex(std::string&& p_index) {
      typename ConfigPointerVector::iterator it  = c_configs.begin(),
                                             end = c_configs.end();
      while( it != end && (*it)->index() != p_index ) {
        ++it;
      }
      return it;
    }
    
    /**
     * Is using config->index() and compares p_index to it.
     * If it matches, it will return the iterator to the corresponding element,
     * else, it will return c_config.end()
     */
    virtual typename ConfigPointerVector::iterator findConfigByIndex(const std::string& p_index) {
      typename ConfigPointerVector::iterator it  = c_configs.begin(),
                                             end = c_configs.end();
      while( it != end && (*it)->index() != p_index ) {
       ++it;
      }
      return it;
    }
    
  protected:
    ConfigVector<T>() {}
    
    ConfigPointerVector c_configs;
    
  private:
  };
    
}} // cloudos::core

#endif
