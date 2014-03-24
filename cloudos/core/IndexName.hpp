
#ifndef CLOUDOS_CORE_INDEX_NAME_HPP__
#define CLOUDOS_CORE_INDEX_NAME_HPP__

#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>

namespace cloudos {
namespace core {
  
  class IndexName;
  
  typedef boost::shared_ptr<IndexName> IndexNamePointer;
  
  /**
   * Why we need this class?
   * You will need this class, if you like to use the container cloudos::core::IndexedQueue
   * At least, it will provide the needed index-infrastructure, required by cloudos::core::IndexedQueue
   * 
   * What is possible with this class?
   * Set/Get a map-friedly index name
   * 
   * Is there a special behaviour, on special circumstances?
   *   - Header only
   *   - you are not allowed to instanciate this class directly!
   *   - moveable (see std::move())
   * 
   * How to use this class as an object?
   * class blubb : public IndexName {
   *  // some implementation here...
   * };
   */
  class IndexName {
  public:
    
    /**
     * Sets a map-friendly index string
     */
    virtual void setIndexName(std::string&& p_index_name);
    
    /**
     * Sets a map-friendly index string
     */
    virtual void setIndexName(const std::string& p_index_name);
    
    /**
     * Will return an map-friendly index string (if set).
     */
    inline const std::string& getIndexName() {
      return c_index_name;
    }
    
  protected:
    IndexName() {
    }
    
    /**
     * An index key for using insite of maps/sets
     */
    std::string c_index_name;
  };
  
}} // cloudos::core


#endif
