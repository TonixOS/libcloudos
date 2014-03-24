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



#ifndef CLOUDOS_CORE_LOGGING_HPP__
#define CLOUDOS_CORE_LOGGING_HPP__

#include <string>
#include <sstream>


#include <boost/move/utility.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/keywords/severity.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/trivial.hpp>

namespace blog = boost::log;

namespace cloudos {
namespace core {
  
  enum severity_level {
    normal,
    info,
    warning,
    error,
    critical
  };
  
  /**
   * To get access to the log object
   * use log::sources::logger_mt& lg = cloudos_log::get();
   * BOOST_LOG(lg) << "log message";
   */
  
  // define our global log instance, named cloudos_log
  BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(cloudos_log, blog::sources::severity_logger_mt< blog::trivial::severity_level >)
  
// our easy way to create a log record
// only use this macro, if you are logging within a class, which is derived from cloudos::core::Logger!!!
// OID == object ID (aka memory address to identify the running object)
  #define LOG(severity) BOOST_LOG_SEV(c_lg, severity) << "[OID:" << this << "] " << "[F:" << __PRETTY_FUNCTION__ << "] "

// some shortcuts
#define LOG_F() LOG(blog::trivial::fatal)
#define LOG_E() LOG(blog::trivial::error)
#define LOG_W() LOG(blog::trivial::warning)
#define LOG_I() LOG(blog::trivial::info)
#define LOG_D() LOG(blog::trivial::debug)
#define LOG_T() LOG(blog::trivial::trace)
  
  /**
   * Why we need this class?
   * It will reduce the amount of code for writing LOG-messages within other classes.
   * It provides an easy to use interface for all objects, which will inherent this class.
   * 
   * What is possible with this class?
   * Inherent this class and you get an easy interface for logging.
   * 
   * How to use this class as an object?
   * Just derive from this class.
   * e.g.: class Command : public Logger { ... };
   */
  class Logger {
  public:
    typedef blog::sources::severity_logger_mt< blog::trivial::severity_level > LogSourceType;
    
  protected:
    /**
     * Our logging object, instanciated globaly; we just use a copy of it...
     */
    //boost::log::sources::logger_mt& c_lg;
    //boost::shared_ptr< blog::core > c_lg;
    LogSourceType& c_lg;
    
  public:
    Logger() : c_lg(cloudos_log::get()) {}
    //Logger() : c_lg(::cloudos::core::cloudos_log::get()) { }
    /*Logger() {
      c_lg = blog::core::get();
    }*/
  };
  
}} // cloudos::core

#endif
