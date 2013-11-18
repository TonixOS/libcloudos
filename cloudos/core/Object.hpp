
#ifndef CLOUDOS_CORE_OBJECT_HPP__
#define CLOUDOS_CORE_OBJECT_HPP__

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
  class Object {
  public:
    
    bool isValid() const { return c_is_valid; };
    
  protected:
    Object();
    
    bool c_is_valid;
  private:
  };
  
}} // cloudos::core

#endif
