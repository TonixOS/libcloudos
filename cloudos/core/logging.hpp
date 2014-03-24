
#ifndef CLOUDOS_CORE_LOGGING_HPP__
#define CLOUDOS_CORE_LOGGING_HPP__

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace log = boost::log;

namespace cloudos {
namespace core {
  
  /**
   * To get access to the log object
   * use log::sources::logger_mt& lg = cloudos_log::get();
   * BOOST_LOG(lg) << "log message";
   */
  
  // define our global log instance, named cloudos_log
  BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(cloudos_log, log::sources::logger_mt)
  
}} // cloudos::core

#endif
