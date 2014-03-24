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



#ifndef CLOUDOS_CORE_CONFIG_HPP__
#define CLOUDOS_CORE_CONFIG_HPP__

#include <sstream>

#include <boost/smart_ptr/shared_ptr.hpp>

#include <google/protobuf/message.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <cloudos/core/Object.hpp>
#include <cloudos/system/File.hpp>

namespace fs = boost::filesystem;
namespace pb = google::protobuf;

namespace cloudos {
namespace core {
  
  /**
   * Why we need this class?
   * You will need this class, if you like to have a nice and simple config
   * settings (google::protobuf::Message) handling for your object.
   * 
   * What is possible with this class?
   * Load/get/set google::protobuf message objects as config source
   * 
   * Is there a special behaviour, on special circumstances?
   *   - Header only
   *   - you are not allowed to instanciate this class directly!
   * 
   * How to use this class as an object?
   * class blubb : public Config {
   *  // some implementation here...
   * };
   */
  template<typename T>
  class Config : public Object {
  public:
    
    typedef boost::shared_ptr<T> ConfigPointer;
    
    /**
     * Will load the config settings from the given path/file
     */
    virtual bool loadConfig(fs::path&& p_config_path) {
      system::File config_file( std::move(p_config_path) );
      
      LOG_I() << "loading config file " << config_file.getFileName();
      
      std::stringstream ss;
      if( config_file.getContent(ss) == false ) {
        LOG_E() << "could not read the config file " << config_file.getFileName() << " Abort!";
        return false;
      }
      
      boost::shared_ptr<T> config( new T() );
      if( pb::TextFormat::ParseFromString(ss.str(), config.get()) == false ) {
        LOG_E() << "parsing config file FAILED! Abort!";
        return false;
      }
      
      c_config = config;
      LOG_I() << "SUCCESS";
      checkIsValid();
      return true;
    }
    
    /**
     * Will load the config settings from the given path/file
     */
    virtual bool loadConfig(const fs::path& p_config_path) {
      system::File config_file( p_config_path );
      
      LOG_I() << "loading config file " << config_file.getFileName();
      
      std::stringstream ss;
      if( config_file.getContent(ss) == false ) {
        LOG_E() << "could not read the config file " << config_file.getFileName() << " Abort!";
        return false;
      }
      
      boost::shared_ptr<T> config( new T() );
      if( pb::TextFormat::ParseFromString(ss.str(), config.get()) == false ) {
        LOG_E() << "parsing config file FAILED! Abort!";
        return false;
      }
      
      c_config = config;
      checkIsValid();
      LOG_I() << "SUCCESS";
      return true;
    }
    
    /**
     * Sets the configuration
     */
    virtual bool setConfig(boost::shared_ptr<T> p_config) {
      LOG_I() << "setting config data...";
      if( !p_config ) {
        LOG_E() << "given config data is invalid (nullptr). Abort!";
        return false;
      }
      c_config = p_config;
      checkIsValid();
      return true;
    }
    
    /**
     * Will return a copy of the configuration
     */
    virtual boost::shared_ptr<T> getConfig() const {
      boost::shared_ptr<T> copy( new T() );
      copy->CopyFrom( *c_config );
      return copy;
    }
    
  protected:
    Config<T>() {
      c_config = boost::shared_ptr<T>( new T() );
    }
    
    boost::shared_ptr<T> c_config;
    
  private:
  };
  
}} // cloudos::core

#endif
