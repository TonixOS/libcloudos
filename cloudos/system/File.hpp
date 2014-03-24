
#ifndef CLOUDOS_SYSTEM_FILE_HPP__
#define CLOUDOS_SYSTEM_FILE_HPP__

#include <string>
#include <sstream>

#include <boost/filesystem.hpp>

#include <cloudos/core/Object.hpp>
#include <cloudos/system/Directory.hpp>

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
   * 
   * TODO: Create system::FileSystemElement to deduplicate code between Directory and File
   */
  class File : public core::Object {
  public:
    
    File( fs::path&& p_file );
    File( const fs::path& p_file );
    
    /**
     * for base init; but you need to drop this object, as you are unable to set it in a valid state!
     * Use std::move() to init your File object!
     * 
     * Like:
     * File file; // always keeping invalid state
     * File file_valid("/path/to/file"); // valid
     * file = std::move(file_valid); // file is now file_valid, so tar is valid! (file_valid migth be invalid now)
     */
    File() {}
    
    /**
     * If the file does not exists, this function will try to create it.
     * 
     * Returns 
     *   0 => on success (exists or could be created)
     *   1 => could not create the file and it does not exist
     *   4 => the path exists, but is not a file
     *   6 => the parent directory is not read/write accessible
     * 
     * other values are unused.
     */
    ushort createIfNotExists();
    
    /**
     * Returns true, if the file is readable.
     * 
     * Will create the file, if it does not exist.
     */
    bool isAccessible();
    
    /**
     * Returns true, if the file is RW able.
     * 
     * Will create the file, if it does not exist.
     */
    bool isWriteable();
    
    /**
     * appends the content of this file to p_storage;
     * Returns false, if something is wrong (file obj is invalid) or happend (read error).
     */
    bool getContent( std::stringstream& p_storage );
    
    /**
     * Will truncate the file and insert the givven content
     */
    bool setContent( std::stringstream&& p_new_content );
    
    /**
     * Will truncate the file and insert the givven content
     */
    bool setContent( std::string&& p_new_content );
    
    /**
     * Appends content to the file
     */
    bool addContent( std::stringstream&& p_add_content );
    
    /**
     * Checks, if the filename ends with the content from p_lookup.
     * 
     * Returns true, if so; else false.
     */
    bool filenameEndsWith( std::string&& p_lookup ) const;
    
    /**
     * If set to true, the file needs to exists, to gain isValid() == true
     * 
     * Default value is false.
     */
    void setRequireExists( bool p_value );
    
    /**
     * Returns the current filename + path
     */
    const std::string& getFileName() const {
      return c_file_str;
    }
    
    /**
     * Returns the current filename + path
     */
    const fs::path& getFileNameAsPath() const {
      return c_file;
    }
    
  protected:
    fs::path c_file;
    
    /**
     * c_file, but as a string
     */
    std::string c_file_str;
    
    /**
     * If set to true, the file needs to exists to gain this->isValid() == true
     * 
     * Default value is false
     */
    bool c_requires_file_exists;
    
    /**
     * Checks, if the given file is valid. This will be controlable by some
     * custom settings.
     * 
     * If this->requireExists() is set, the file needs to exists
     * and at least readable.
     * 
     * This function will control c_is_valid automaticly.
     */
    virtual void checkIsValid();
  private:
    
    /**
     * Writes the given p_content to p_stream.
     */
    bool insertContent( std::ofstream& p_stream, std::stringstream&& p_content );
    
    /**
     * Writes the given p_content to p_stream.
     */
    bool insertContent( std::ofstream& p_stream, std::string&& p_content );
    
    /**
     * Cache our validation checks
     */
    int c_check_cache;
  };
  
}} // cloudos::system

#endif
  