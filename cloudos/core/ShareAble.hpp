
#ifndef CLOUDOS_CORE_SHARE_ABLE_HPP__
#define CLOUDOS_CORE_SHARE_ABLE_HPP__


namespace cloudos {
namespace core {
  
  /**
   * Why we need this class?
   * This class provides you with all functionality, required by an shared object.
   * You are not allowed to instanciate this class directly!
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * class YourClass : public ShareAble { ... };
   * 
   */
  class ShareAble {
  public:
  protected:
    ShareAble() {
      c_instance_counter = 0;
    }
    inline void incrementInstanceCount() {
      ++c_instance_counter;
    }
    inline void decrementInstanceCount() {
      if(c_instance_counter > 0) {
        --c_instance_counter;
      }
    }
    inline uint getInstanceCount() {
      return c_instance_counter;
    }
  private:
    uint c_instance_counter;
  };
}}
#endif
