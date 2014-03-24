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



#include <algorithm>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
namespace algo = boost::algorithm;
namespace fs = boost::filesystem;

// for access() call
extern "C" {
#include <unistd.h>
}

#include <cloudos/system/Command.hpp>

namespace cloudos {
namespace system {
  
  Command::Command() {
    init();
  }
  
  Command::Command ( const std::string& p_command_name ) {
    init();
    setCommand(p_command_name);
  }
  
  Command::Command ( const Command& p_cmd ) {
    init();
    setCommand(            p_cmd.getCommandName() );
    setArguments(          p_cmd.getArguments()   );
    setEnvironment(        p_cmd.getEnvironment() );
    c_last_return_value =  p_cmd.c_last_return_value;
    c_stderr            << p_cmd.c_stderr.str();
    c_stdout            << p_cmd.c_stdout.str();
    c_stdin             =  p_cmd.c_stdin;
    c_ctx               =  p_cmd.c_ctx;
  }

  const std::stringstream& Command::getStdoutOutput() const {
    return c_stdout;
  }
  
  const std::stringstream& Command::getStderrOutput() const {
    return c_stderr;
  }
  
  void Command::setOpenCommandStreams ( unsigned short p_type ) {
    if( p_type & STDOUT_OUTPUT ) {
      c_ctx.stdout_behavior = ps::capture_stream();
    } else {
      c_ctx.stdout_behavior = ps::silence_stream();
    }
    
    if( p_type & STDERR_OUTPUT ) {
      c_ctx.stderr_behavior = ps::capture_stream();
    } else {
      c_ctx.stderr_behavior = ps::silence_stream();
    }
    
    if( p_type & STDIN_INPUT ) {
      c_ctx.stdin_behavior = ps::capture_stream();
    } else {
      c_ctx.stdin_behavior = ps::silence_stream();
    }
    
    c_enabled_streams = p_type;
  }
  
  bool Command::setCommand ( const std::string& p_command_name ) {
    return setCommand( std::move(std::string(p_command_name)) );
  }
  
  bool Command::setCommand ( std::string && p_command_name ) {
    if( fs::exists( p_command_name ) == false && toBinFromPATH( p_command_name ) == false ) {
      LOG_E() << "could not find command file (neither in $PATH) " << p_command_name;
      return false;
    }
    
    if( access(p_command_name.c_str(), X_OK) != 0 ) {
      LOG_E() << "command " << p_command_name << " is not executable!";
      return false;
    }
    
    c_cmd = std::move( p_command_name );
    c_is_valid = true;
    return true;
  }
  
  const std::string& Command::getCommandName() const {
    return c_cmd;
  }

  void Command::addArgument ( const std::string& p_arg ) {
    c_arguments.push_back(p_arg);
  }
  
  void Command::addArgument ( std::string && p_arg ) {
    c_arguments.push_back( std::move(p_arg) );
  }

  void Command::setArguments ( const std::vector< std::string >& p_arguments ) {
    c_arguments.clear();
    c_arguments = p_arguments;
    //c_arguments.insert( c_arguments.begin(), "" ); // this is our workaround for a bug within boost::process
    //std::copy( p_arguments.cbegin(), p_arguments.cend(), c_arguments.begin() );
  }
  
  void Command::setArguments ( std::vector< std::string > && p_arguments ) {
    c_arguments = std::move( p_arguments );
    c_arguments.insert( c_arguments.begin(), std::move(std::string("")) );
  }
  
  const std::vector< std::string >& Command::getArguments() const {
    return c_arguments;
  }

  void Command::clearArguments() {
    c_arguments.clear();
    c_arguments.push_back(""); // the first needs to be empty, this seems to be a bug within boost::process...
  }
  
  void Command::setEnvironmentVar ( const std::string& p_name, const std::string& p_value ) {
    c_ctx.environment.insert(ps::environment::value_type(p_name, p_value));
  }
  
  void Command::setEnvironmentVar ( std::string && p_name, std::string && p_value ) {
    c_ctx.environment.insert(std::move(ps::environment::value_type(p_name, p_value)));
  }
  
  void Command::removeEnvironmentVar ( const std::string& p_name ) {
    removeEnvironmentVar( std::move(p_name) );
  }
  
  void Command::removeEnvironmentVar ( std::string && p_name ) {
    ps::environment::iterator it;
    it = c_ctx.environment.find( p_name );
    if( it != c_ctx.environment.end() ) {
      c_ctx.environment.erase(it);
    }
  }
  
  void Command::clearEnvironment() {
    c_ctx.environment.clear();
  }
  
  void Command::setReturnValuesOK ( const std::vector< int >& p_values ) {
    c_return_values_ok = p_values;
  }
  
  void Command::addReturnValueOK ( int p_value ) {
    c_return_values_ok.push_back(p_value);
  }

  Command& Command::operator<< ( const std::string& p_element ) {
    if( c_is_valid == false ) {
      setCommand(p_element);
    } else {
      addArgument(p_element);
    }
    return *this;
  }
  
  Command& Command::operator<< ( std::string&& p_element ) {
    if( c_is_valid == false ) {
      setCommand( std::move(p_element) );
    } else {
      addArgument( std::move(p_element) );
    }
    return *this;
  }
  
  Command& Command::operator<< ( const fs::path& p_element ) {
    if( c_is_valid == false ) {
      setCommand( p_element.string() );
    } else {
      addArgument( p_element.string() );
    }
    return *this;
  }
  
  Command& Command::operator<< ( fs::path&& p_element ) {
    if( c_is_valid == false ) {
      setCommand( p_element.string() );
    } else {
      addArgument( p_element.string() );
    }
    return *this;
  }
  
  Command& Command::operator<< ( const char* p_element ) {
    std::string tmp(p_element);
    if( c_is_valid == false ) {
      setCommand( std::move(tmp) );
    } else {
      addArgument( std::move(tmp) );
    }
    return *this;
  }

  bool Command::start() {
    if( reportInvalidObjectError( __PRETTY_FUNCTION__ ) ) {
      return false;
    }
    if( c_is_running ) {
      return true;
    }
    
    // workaround for boost::process
    if( c_arguments.empty() ) {
      c_arguments.push_back("");
    }
    
    // if we are requested to run within an chroot,
    // prepare arguments and change the command to chroot
    if( c_chroot ) {
      LOG_I() << "chroot in " << c_chroot->getRootPath() << " requested";
      // create new arguements vector
      std::vector<std::string> args;
      args.push_back( "" );
      args.push_back( c_chroot->getRootPath() );
      args.push_back( c_cmd );
      // remove the first, empty entry
      args.insert( args.end(), ++(c_arguments.begin()), c_arguments.end() );
      
      bool chroot_state = c_chroot->isEnabled();
      if( chroot_state == false && c_chroot->enable() == false ) {
        LOG_E() << "failed to set up chroot, so I can't execute the command! Abort launching the command...";
        return false;
      }
      
      LOG_I() << "starting command " << c_chroot->getChrootBin()
              << " \"" << algo::join( args, "\" \"" ) << '"' << "...";
      c_child.reset( ps::launch(c_chroot->getChrootBin(), args, c_ctx) );
      
      if( chroot_state == false ) {
        c_chroot->disable(); // restore chroot state
      }
      
    } else {
      LOG_I() << "starting command " << c_cmd << " \"" << algo::join( c_arguments, "\" \"" ) << '"' << "...";
      c_child.reset( ps::launch(c_cmd, c_arguments, c_ctx) );
    }
    
    LOG_I() << "command started with PID: " << c_child->get_id();
    
    sendStdInputData();
    
    c_is_running = true;
    return true;
  }

  short int Command::waitUntilFinished() {
    if( start() == false ) {
      return 1;
    }
    
    // close stream, if some data were send to the program
    if( c_stdin_close_needed ) {
      ps::postream& stdin = c_child->get_stdin();
      stdin.close();
      c_stdin_close_needed = false;
    }
    ps::status status = c_child->wait();
    
    // clear streams, to remove possibly old data
    c_stdout.clear();
    c_stderr.clear();
    
    if( c_ctx.stdout_behavior.get_type() == ps::capture_stream().get_type() ) {
      getStream( c_child->get_stdout(), c_stdout );
      
      std::string out = c_stdout.str();
      if( out != "\n" ) { // if we got something on stderr...
        LOG_D() << "wrote on standard output:";
        LOG_D() << logCommandOutputStream(blog::trivial::debug, c_stdout);
      }
    }
    if( c_ctx.stderr_behavior.get_type() == ps::capture_stream().get_type() ) {
      getStream( c_child->get_stderr(), c_stderr );
      
      std::string errs = c_stderr.str();
      if( errs != "\n" ) { // if we got something on stderr...
        LOG_E() << "wrote on error output:";
        LOG_E() << logCommandOutputStream(blog::trivial::info, c_stderr);
      }
    }
    
    c_is_running = false;
    c_last_return_value = getMappedReturnValue( status.exit_status() );
    LOG_I() << "cmd exited with mapped return value: " << c_last_return_value;
    return c_last_return_value;
  }
  
  void Command::setDescription ( const std::string& p_description ) {
    c_description = p_description;
  }
  
  void Command::setChRootController ( ChRootControllerPointer p_chroot_ctl ) {
    c_chroot = p_chroot_ctl;
  }
  
  bool Command::toBinFromPATH ( std::string& p_search_bin ) {
    std::vector<std::string> paths;
    algo::split(paths, ps::self::get_environment()["PATH"], boost::is_any_of(":"));
    
    for(auto& path : paths) {
      fs::path test( path );
      test = test / p_search_bin;
      if( fs::exists( test ) ) {
        p_search_bin = test.string();
        return true;
      }
    }
    return false;
  }
  
  void Command::setInputStreamData ( std::string && p_stream_data ) {
    c_stdin = std::move( p_stream_data );
    
    if( (c_enabled_streams & STDIN_INPUT) == 0 ) { // if input stream is currently not set...
      setOpenCommandStreams( c_enabled_streams | STDIN_INPUT );
    }
    
    // send data directly, if program is already running
    if( c_is_running ) {
      sendStdInputData();
    }
  }
  
  // 
  // P R O T E C T E D
  // 
  int Command::getMappedReturnValue ( int p_command_return_value ) {
    return ( std::find( c_return_values_ok.begin(), c_return_values_ok.end(), p_command_return_value ) ==
    c_return_values_ok.end() )
        ? p_command_return_value
        : 0;
  }

  // 
  // P R I V A T E
  // 
  void Command::getStream( ps::pistream& p_output, std::stringstream& p_stream ) {
    std::string line;
    while( p_output.good() ) {
      std::getline(p_output, line);
      p_stream << line << std::endl;
      line.clear();
    }
  }

  void Command::init() {
    c_ctx.environment = ps::self::get_environment(); // set the environment of the current running program
    c_enabled_streams = 0;
    setOpenCommandStreams();
    clearArguments();
    c_is_running = false;
    c_description = std::string();
    c_stdin = std::string();
    c_chroot = ChRootControllerPointer();
    c_return_values_ok.clear();
    c_return_values_ok.push_back(0);
    c_last_return_value = 0;
    c_stdin_close_needed = false;
  }
  
  char Command::logCommandOutputStream ( blog::trivial::severity_level p_level, std::stringstream& p_stream ) {
    std::string line;
    while( !std::getline(p_stream, line).eof() ) {
      LOG(p_level) << line;
    }
    return ' ';
  }
  
  void Command::sendStdInputData() {
    if( c_stdin.empty() == false ) {
      LOG_D() << "sending stdin data to program...";
      
      ps::postream& stdin = c_child->get_stdin();
      stdin << c_stdin;
      
      c_stdin.clear();
      c_stdin_close_needed = true;
      
      LOG_D() << "sending stdin data done";
    }
  }
  
}} // cloudos::system
