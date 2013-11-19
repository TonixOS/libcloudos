
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

  const std::stringstream& Command::getStdoutOutput() const {
    return c_stdout;
  }
  
  void Command::setCaptureOutput ( unsigned short p_type ) {
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
  }
  
  void Command::setCommand ( const std::string& p_command_name ) {
    c_cmd = p_command_name;
    c_is_valid = true;
  }

  void Command::addArgument ( const std::string& p_arg ) {
    c_arguments.push_back(p_arg);
  }

  void Command::setArguments ( const std::vector< std::string >& p_arguments ) {
    c_arguments = p_arguments;
    c_arguments.insert( c_arguments.begin(), "" ); // this is our workaround for a bug within boost::process
  }

  void Command::clearArguments() {
    c_arguments.clear();
    c_arguments.push_back(""); // the first needs to be empty, this seems to be a bug within boost::process...
  }
  
  void Command::setEnvironmentVar ( const std::string& p_name, const std::string& p_value ) {
    c_ctx.environment.insert(ps::environment::value_type(p_name, p_value));
  }
  
  void Command::removeEnvironmentVar ( const std::string& p_name ) {
    c_ctx.environment.erase(p_name);
  }
  
  void Command::clearEnvironment() {
    c_ctx.environment.clear();
  }

  Command& Command::operator<< ( const std::string& p_element ) {
    if( c_is_valid == false ) {
      setCommand(p_element);
    } else {
      addArgument(p_element);
    }
    return *this;
  }

  bool Command::start() {
    if( c_is_valid == false ) {
      return false;
    }
    if( c_is_running ) {
      return true;
    }
    
    c_child.reset( ps::launch(c_cmd, c_arguments, c_ctx) );
    c_is_running = true;
    return true;
  }

  short int Command::waitUntilFinished() {
    if( start() == false ) {
      return false;
    }
    
    ps::status status = c_child->wait();
    
    if( c_ctx.stdout_behavior.get_type() == ps::capture_stream().get_type() ) {
      getStream( c_child->get_stdout(), c_stdout );
    }
    if( c_ctx.stderr_behavior.get_type() == ps::capture_stream().get_type() ) {
      getStream( c_child->get_stderr(), c_stderr );
    }
    
    c_is_running = false;
    return status.exit_status();
  }
  
  void Command::setDescription ( const std::string& p_description ) {
    c_description = p_description;
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
    setCaptureOutput( STDOUT_OUTPUT | STDERR_OUTPUT );
    clearArguments();
    c_is_running = false;
    c_description = std::string("");
  }
  
}} // cloudos::system
