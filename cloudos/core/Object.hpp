
#ifndef CLOUDOS_CORE_OBJECT_HPP__
#define CLOUDOS_CORE_OBJECT_HPP__

#include <cloudos/core/logging.hpp>

namespace cloudos {
namespace core {
  
  /**
   * Why we need this class?
   * This will add a few special, generic object variables/functions
   * Esspecialy for debugging...
   * 
   * What is possible with this class?
   * Just inherent this class
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * Don't do this
   */
  class Object : public Logger {
  public:
    
    virtual bool isValid() const { return c_is_valid; }
    
  protected:
    Object();
    ~Object();
    
    bool c_is_valid;
    
    /**
     * On default, this function will do nothing...
     * 
     * But it should be used to set c_is_valid.
     */
    virtual void checkIsValid() {}
    
    virtual bool reportInvalidObjectError( const std::string& p_from_function ) const;
  private:
  };
  
}} // cloudos::core

#endif
