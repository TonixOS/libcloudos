
// for backtrace
extern "C" {
#include <execinfo.h>
}

#include <cloudos/core/Object.hpp>

namespace cloudos {
namespace core {
  
  Object::Object() {
    c_is_valid = false;
  }
  
  Object::~Object() {
    LOG_T() << "good state: destoying";
  }

  bool Object::reportInvalidObjectError ( const std::string& p_from_function ) const {
    if( c_is_valid ) {
      return false; // we don't need to report an error; object is valid
    }
    
    LOG_E() << "Object is invalid, but the following function requires a valid instance: " << p_from_function;
    LOG_E() << "Abort!";
    
    char** backtrace_lines;
    void *backtrace_elements[20];
    size_t available_stack_elements;
    
    LOG_E() << "running backtrace:";
    
    // get void*'s for up to 20 entries on the stack
    // size will have the real number of entries, stored in array
    // we need to reference from start (with the leading ::), as this function is also called "backtrace()"
    available_stack_elements = ::backtrace(backtrace_elements, 20);
    
    // print out all the frames to LOG
    backtrace_lines = backtrace_symbols(backtrace_elements, available_stack_elements);
    
    for(int i = 0; i < available_stack_elements; ++i) {
      LOG_E() << backtrace_lines[i];
    }
    
    return true; // we reported an error; object is invalid
  }
  
}}
