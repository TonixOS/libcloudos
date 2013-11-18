
#ifndef CLOUDOS_SYSTEM_FILE_HPP__
#define CLOUDOS_SYSTEM_FILE_HPP__

#include <string>
#include <sstream>

#include <boost/filesystem.hpp>

#include <cloudos/core/Object.hpp>

namespace fs = boost::filesystem;

namespace cloudos {
namespace system {
  
  /**
   * Why we need this class?
   * To control and handle system files. We like to handle special files in a special manner.
   * So this will be our base class with some basic functions.
   * 
   * What is possible with this class?
   * Reading/Writing/Accessing files and their meta-data
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * File file("/path/to/file/filename.fileextension")
   */
  class File : public core::Object {
  public:
    File( const fs::path& p_file );
    
    /**
     * appends the content of this file to p_storage;
     * Returns false, if something is wrong (file obj is invalid) or happend (read error).
     */
    bool getContent( std::stringstream& p_storage );
    
  protected:
    fs::path c_file;
  private:
  };
  
}} // cloudos::system

#endif
  