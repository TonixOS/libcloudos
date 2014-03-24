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



#ifndef CLOUDOS_SYSTEM_COMMAND_HPP__
#define CLOUDOS_SYSTEM_COMMAND_HPP__

#include <list>
#include <ostream>
#include <string>
#include <sstream>
#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/process.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>

#include <cloudos/core/Object.hpp>
#include <cloudos/core/IndexName.hpp>
#include <cloudos/system/ChRootController.hpp>

namespace fs = boost::filesystem;
namespace ps = boost::process;
namespace sig = boost::signals2;

namespace cloudos {
namespace system {
  
  // STDIN_INPUT will be automaticly enabled, if input data is set...
  enum CommandStreamTypes {
    STDOUT_OUTPUT  = 1,
    STDERR_OUTPUT  = 2,
    STDIN_INPUT    = 4,
    CMD_STREAM_ALL = 7
  };
  
  class Command;
  typedef boost::shared_ptr<Command> CommandPointer;
  typedef std::list<CommandPointer>  CommandPointerList;
  typedef std::vector<Command>       CommandVector;
  
  // our signal types
  // typedef sig::signal<void (const Command*)> SignalCommandType;
  //typedef SignalCommandType::slot_type SignalCommandSlotType;
  
  /**
   * Why we need this class?
   * This class will reduce the amount of code lines a lot!
   * It will improve the readability of command execution a lot!
   * 
   * What is possible with this class?
   * Create a command line (command and arguments) and control its execution.
   * This class is limited to standard streams.
   * (for custom streams, or process pipes, please use boost::process instead)
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * Command cmd;
   * cmd << "/bin/tar" << "-xf" << "file.tar";
   * cmd.disableCaptureOutput(); // is enabled by default
   * cmd.start();
   * cmd.waitUntilFinished();
   * 
   * TODO: Enable timeout settings on each execution. 2min on default
   *       http://www.highscore.de/boost/gsoc2010/process/user_guide.html#boost_process.user_guide.waiting
   * TODO: Add signal "commandFinished(bool, cmd_name, index_name)"
   */
  class Command : public core::Object, public core::IndexName {
  public:
    Command();
    Command(const std::string& p_command_name);
    Command(const Command& p_cmd);
    
    /**
     * returns the STDOUT data of the process.
     * Will return an empty stringstream, if STDOUT_OUTPUT capturing is disabled or command is not executed
     */
    const std::stringstream& getStdoutOutput() const;
    
    /**
     * returns the STDERR data of the process
     * Will return an empty stringstream, if STDERR_OUTPUT capturing is disabled or command is not executed
     */
    const std::stringstream& getStderrOutput() const;
    
    /**
     * Disables the capture of messages by the command
     * Default: STDOUT and STDERR are captured
     * 
     * TODO: Implement handling STDIN_INPUT (currently not supported)
     */
    void setOpenCommandStreams( unsigned short p_type = (STDOUT_OUTPUT | STDERR_OUTPUT));
    
    /**
     * Set the command, this command will be executed, after start() is called
     * The given command will only be set, if it exists and is executeable,
     * if not, it will return false
     */
    bool setCommand( const std::string& p_command_name );
    
    /**
     * Our C++11 version of setCommand
     */
    bool setCommand( std::string&& p_command_name );
    
    /**
     * get the given command name+path
     */
    const std::string& getCommandName() const;
    
    /**
     * Adds an argument to the command argument-list
     */
    void addArgument( const std::string& p_arg );
    
    /**
     * C++11 version of addArgument
     */
    void addArgument( std::string&& p_arg );
    
    /**
     * Will replace the current set of arguments, with p_arguments
     */
    void setArguments( const std::vector<std::string>& p_arguments );
    
    /**
     * C++11 version of setArguments
     */
    void setArguments( std::vector<std::string>&& p_arguments );
    
    /**
     * Returns all given arguments
     */
    const std::vector<std::string>& getArguments() const;
    
    /**
     * Empties argument list
     */
    void clearArguments();
    
    /**
     * Sets an environment variable for the upcomming process.
     * Will overwrite the value, if p_name is already set!
     */
    void setEnvironmentVar(const std::string& p_name, const std::string& p_value = std::string(""));
    
    /**
     * C++11 version of setEnvironmentVar
     */
    void setEnvironmentVar(std::string&& p_name, std::string&& p_value = std::move(std::string("")));
    
    /**
     * Will replace the current environment variable with the new one
     */
    void setEnvironment( const ps::environment& p_env ) { c_ctx.environment = p_env; }
    
    /**
     * C++11 version of setEnvironment
     */
    void setEnvironment( ps::environment&& p_env ) { c_ctx.environment = std::move(p_env); }
    
    /**
     * If the environment variable exists, it will be unset.
     */
    void removeEnvironmentVar(const std::string& p_name);
    
    /**
     * C++11 version of removeEnvironmentVar
     */
    void removeEnvironmentVar(std::string&& p_name);
    
    /**
     * Will remove all environment variables.
     */
    void clearEnvironment();
    
    /**
     * Will return the current command environment variables.
     */
    const ps::environment& getEnvironment() const { return c_ctx.environment; }
    
    /**
     * Will set all valid return values, which indicates an successful ending of the command.
     * Default is just "0" (Zero).
     * To let all return values be an error state, set an empty vector.
     */
    void setReturnValuesOK( const std::vector<int>& p_values );
    
    /**
     * Will add an valid ending (return) value for the command.
     */
    void addReturnValueOK( int p_value );
    
    /**
     * If no command name is set, the first input will be used as command name.
     * All other inputs are treated as arguments.
     * 
     * TODO: allow fs::path and int values too...
     *       on fs::path, check if it exists... is this always wanted?
     */
    Command& operator<<( const std::string& p_element );
    
    /**
     * see other operator<<
     * This is just a fast version of it
     */
    Command& operator<<( std::string&& p_element );
    
    /**
     * see other operator<<
     */
    Command& operator<<( const fs::path& p_element );
    
    /**
     * see other operator<<
     */
    Command& operator<<( fs::path&& p_element );
    
    /**
     * see other operator<<
     */
    Command& operator<<( const char* p_element );
    
    /**
     * Returns true, if starting the command was successful
     * (this does not mean, that the command finished successfuly!)
     * 
     * After calling start(), the command will be executed within its own process.
     * So start() will return immediately, after launching the process.
     * 
     * start() won't start a second instance of the command, so it's save to call it multiple times.
     * Will return true, if the process is already started...
     */
    bool start();
    
    /**
     * will be a signal, if it's ready
     */
    //sig::connection slotAddCommandStarted();
    
    /**
     * returns, if the process is running (and waitUntilFinished() wasn't called jet)
     */
    bool isRunning() { return c_is_running; }
    
    /**
     * After calling start(), you need to call waitUntilFinished()
     * to be sure, that the command finished.
     * 
     * It will return 0 (Zero), if the return value of the command is
     * one of the given successful-return-values, else the return value
     * of the command will be returned.
     */
    short waitUntilFinished();
    
    /**
     * Set a human friendly description, what this command is about.
     * 
     * This description will be used in debug logs and on other interaction with man kind :-)
     */
    void setDescription( const std::string& p_description );
    
    /**
     * Returns the human friendly command description.
     * 
     * See setDescription()
     */
    const std::string& getDescription() const { return c_description; }
    
    /**
     * If you like to execute this command within an chroot,
     * you will need to provide an ChRootController to it.
     * This object might call "setup()" by itself, if start() is called,
     * but the chroot is not set up jet.
     * The ChRootController will be copied and the copy will be destroyed,
     * while this object is destroying.
     */
    void setChRootController(ChRootControllerPointer p_chroot_ctl);
    
    /**
     * Will return the exit value of the last running job.
     */
    int getLastExitCode() const { return c_last_return_value; }
    
    /**
     * Static function to find the givven command within $PATH env var.
     * If the binary was found within $PATH, the new absolute path+binary-name will be stored in p_search_bin.
     * Else, p_search_bin won't change!
     * 
     * Returns true, if the binary was found within $PATH, else false.
     */
    static bool toBinFromPATH( std::string& p_search_bin );
    
    /**
     * Sets the stdin stream data. If stdin data is not empty; the data will be send to
     * the running program automaticly!
     * The data will be cleared, after sending it succeed!
     * 
     * If set, STDIN_INPUT will automaticly be opened!
     */
    void setInputStreamData( std::string&& p_stream_data );
    
  protected:
    
    /**
     * Command name
     */
    std::string c_cmd;
    
    /**
     * Command arguments
     */
    std::vector<std::string> c_arguments;
    
    /**
     * All valid return values, which will indicate an successful ending of the process
     */
    std::vector<int> c_return_values_ok;
    
    /**
     * We need to wrap ps::child in boost::optional, because ps::child
     * needs to be instanciated immediately (but we won't do this :-))
     * 
     * This is the handle on the process of the command.
     */
    boost::optional<ps::child> c_child;
    
    /**
     * The command "context". Settings like "capture stdout", "env vars"
     * will be stored here.
     */
    ps::context c_ctx;
    
    /**
     * STDOUT data from the process, if STDOUT_OUTPUT is on
     */
    std::stringstream c_stdout;
    
    /**
     * STDERR data from the process, if STDERR_OUTPUT is on
     */
    std::stringstream c_stderr;
    
    /**
     * STDIN content
     * If not empty, the content will be streamed to the running program.
     */
    std::string c_stdin;
    
    /**
     * Set to true, if an stdin was opened and needs to be closed before c_child.wait() is called
     */
    bool c_stdin_close_needed;
    
    /**
     * Which stream states are set...
     */
    unsigned short c_enabled_streams;
    
    /**
     * A human friendly description, what this command is about
     */
    std::string c_description;
    
    /**
     * Will be active, if we need to chroot our command execution.
     */
    ChRootControllerPointer c_chroot;
    
    /**
     * Will check the return value against c_return_values_ok.
     * If the given value is within c_return_values_ok, it will return 0
     * else, the given return value.
     */
    int getMappedReturnValue( int p_command_return_value );
    
  private:
    bool c_is_running;
    
    /**
     * The exit value, from the last running command instance
     */
    int c_last_return_value;
    
    /**
     * Will read the stream data
     */
    void getStream( ps::pistream& p_output, std::stringstream& p_stream );
    
    /**
     * Initiate the class' members
     */
    void init();
    
    /**
     * Logs stream data from given stream with given log-level
     * 
     * Just returns a char, because we like to use logCommandOutputStream() in a stream construct...
     */
    char logCommandOutputStream( blog::trivial::severity_level p_level, std::stringstream& p_stream );
    
    /**
     * Will send the data within c_stdin to the running c_child.
     * 
     * If c_stdin is empty, no data will be send...
     */
    void sendStdInputData();
  };
  
}} // cloudos::system

#endif
  