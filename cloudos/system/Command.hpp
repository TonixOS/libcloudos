
#ifndef CLOUDOS_SYSTEM_COMMAND_HPP__
#define CLOUDOS_SYSTEM_COMMAND_HPP__

#include <ostream>
#include <string>
#include <sstream>
#include <vector>

#include <boost/process.hpp>
#include <boost/optional.hpp>

#include <cloudos/core/Object.hpp>

namespace ps = boost::process;

namespace cloudos {
namespace system {
  
  enum CommandStreamTypes {
    STDOUT_OUTPUT  = 1,
    STDERR_OUTPUT  = 2,
    STDIN_INPUT    = 4,
    CMD_STREAM_ALL = 7
  };
  
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
   * TODO: Check the env-path variable and check, if the given command exists and we will be able to execute it.
   * 
   * TODO: Implement Debug LOG (with PID, command description, command line, command output)
   * 
   * TODO: Add command description member (+get/set) => this way, we are able to print human friendly output
   *       and are able to understand, what this command is about
   */
  class Command : public core::Object {
  public:
    Command();
    Command(const std::string& p_command_name);
    
    /**
     * returns the STDOUT data of the process.
     * Will return an empty stringstream, if STDOUT_OUTPUT capturing is disabled
     */
    const std::stringstream& getStdoutOutput() const;
    
    /**
     * Disables the capture of messages by the command
     * Default: STDOUT and STDERR are captured
     * 
     * TODO: Implement handling STDIN_INPUT (currently not supported)
     */
    void setCaptureOutput( unsigned short p_type = (STDOUT_OUTPUT | STDERR_OUTPUT));
    
    /**
     * Set the command, this command will be executed, after start() is called
     */
    void setCommand( const std::string& p_command_name );
    
    /**
     * Adds an argument to the command argument-list
     */
    void addArgument( const std::string& p_arg );
    
    /**
     * Will replace the current set of arguments, with p_arguments
     */
    void setArguments( const std::vector<std::string>& p_arguments );
    
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
     * Will replace the current environment variable with the new one
     */
    void setEnvironment( const ps::environment& p_env ) { c_ctx.environment = p_env; }
    
    /**
     * If the environment variable exists, it will be unset.
     */
    void removeEnvironmentVar(const std::string& p_name);
    
    /**
     * Will remove all environment variables.
     */
    void clearEnvironment();
    
    /**
     * Will return the current command environment variables.
     */
    const ps::environment& getEnvironment() { return c_ctx.environment; }
    
    /**
     * If no command name is set, the first input will be used as command name.
     * All other inputs are treated as arguments.
     */
    Command& operator<<( const std::string& p_element );
    
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
     * returns, if the process is running (and waitUntilFinished() wasn't called jet)
     */
    bool isRunning() { return c_is_running; }
    
    /**
     * After calling start(), you need to call waitUntilFinished()
     * to be sure, that the command finished.
     * 
     * It will return the exit-code of the process.
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
    const std::string& getDescription() { return c_description; }
    
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
     * A human friendly description, what this command is about
     */
    std::string c_description;
  private:
    bool c_is_running;
    
    /**
     * Will read the stream data
     */
    void getStream( ps::pistream& p_output, std::stringstream& p_stream );
    
    /**
     * Initiate the class' members
     */
    void init();
  };
  
}} // cloudos::system

#endif
  