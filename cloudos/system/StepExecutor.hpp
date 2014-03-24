
#ifndef CLOUDOS_SYSTEM_INSTALLER_STEP_ELEMENT_HPP__
#define CLOUDOS_SYSTEM_INSTALLER_STEP_ELEMENT_HPP__

#include <string>
#include <vector>
#include <functional>

#include <boost/function.hpp>
#include <boost/container/stable_vector.hpp>

#include <cloudos/core/Object.hpp>
#include <cloudos/core/IndexName.hpp>

namespace cloudos {
namespace system {
  
  class StepExecutor;
  typedef boost::container::stable_vector< StepExecutor > InstallerStepVector;
  
  /**
   * Why we need this class?
   * To wrap an installer step, but be able to control validity.
   * The callback() function needs to be secured - this is the reason for a specialized class.
   * 
   * What is possible with this class?
   * Create a callback with a name and a description; call the callback in a secure way.
   * 
   * Is there a special behaviour, on special circumstances?
   * Non known
   * 
   * How to use this class as an object?
   * StepExecutor e;
   * e.setIndexName( "indexname" );
   * e.setCallback( &functionToCall );
   * if( e.isValid() )
   *   e.run();
   * 
   * TODO: implement "isAlreadyExecuted()"
   */
  class StepExecutor : public core::Object, public core::IndexName {
  public:
    StepExecutor();
    
    /**
     * Sets p_index as machine friendly step name
     * Sets p_callback for a function callback, which will do the step
     * Sets p_desc as a human friendly step name
     */
    StepExecutor(std::string&& p_index, boost::function<bool ()>&& p_callback, std::string&& p_desc = std::move( std::string() ));
    /*
    StepExecutor( StepExecutor&& p_from );
    StepExecutor& operator=(StepExecutor&& p_from);*/
    
    /**
     * Sets p_index as machine friendly step name
     */
    virtual inline void setIndexName(std::string&& p_index_name) {
      cloudos::core::IndexName::setIndexName( std::move(p_index_name) );
      checkIsValid();
    }
    
    /**
     * Sets p_index as machine friendly step name
     */
    virtual inline void setIndexName(const std::string& p_index_name) {
      cloudos::core::IndexName::setIndexName( p_index_name );
      checkIsValid();
    }
    
    /**
     * Sets p_callback for a function callback, which will do the step
     */
    void setCallback( boost::function<bool ()>&& p_callback );
    
    /**
     * Sets p_desc as a human friendly step name
     */
    void setDescription( std::string&& p_desc );
    
    /**
     * Will execute the function, which is required to run to accomplish this step.
     */
    bool run();
    
  private:
    /**
     * human friendly name of this element
     */
    std::string c_desc;
    
    /**
     * Will be the function, which should be called if we like to accomplish this state
     */
    boost::function<bool ()> c_callback;
    
    void checkIsValid();
  };
  
}}

#endif
