
#ifndef CLOUDOS_SYSTEM_DIRECTORY_HPP__
#define CLOUDOS_SYSTEM_DIRECTORY_HPP__

#include <vector>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <cloudos/core/Object.hpp>

namespace cloudos {
namespace system {
  
  enum FileLookupMode {
    LOOKUP_BY_REGEX      = 1,
    LOOKUP_BY_STARTSWITH = 2,
    LOOKUP_BY_ENDSWITH   = 4,
    LOOKUP_BY_FULLMATCH  = 8
  };
  
  /**
   * we need this enum, as fs::file_type is not "bit-operate-able"
   * This way, we are able to to scan for multiple file-types at once...
   */
  enum FileType {
    FILE_TYPE_REGULAR   =   1,
    FILE_TYPE_SYMLINK   =   2,
    FILE_TYPE_DIRECTORY =   4,
    FILE_TYPE_ALL       =   7
  };
  
  /**
   * Why we need this class?
   * Controls a directory and gives us some tools for accessing meta-/data from the directory.
   * 
   * What is possible with this class?
   * Check directory access and search for files within this directory.
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * Directory dir("/path/to/dir");
   * std::vector< fs::path > search_result;
   * if( dir.isAccessible() )
   *   findFiles( search_result, "pattern_to_look_for" );
   * // do something with the elements in search_result
   */
  class Directory : public core::Object {
  public:
    Directory( const fs::path& p_dir );
    
    /**
     * for base init; but you need to drop this object, as you are unable to set it in a valid state!
     * Use std::move() to init your Directory object!
     * 
     * Like:
     * Directory dir; // always keeping invalid state
     * Directory dir_valid("/path/to/directory"); // valid
     * dir = std::move(dir_valid); // dir is now dir_valid, so dir is valid! (dir_valid migth be invalid now)
     */
    Directory() {}
    
    /**
     * Copy Constructor
     */
    Directory( const Directory& p_from_directory );
    
    Directory& operator=(Directory&& p_from_directory);
    Directory& operator=(const Directory& p_from_directory);
    
    /**
     * If the directory does not exists, this function will try to create it.
     * 
     * Returns 
     *   0 => on success (exists or could be created)
     *   1 => could not create directory and it does not exist
     *   4 => the path exists, but is not a directory
     *   6 => the directory is not read/write accessible
     * 
     * other values are unused.
     */
    int createIfNotExists();
    
    /**
     * Returns true, if the directory is RX able.
     * 
     * Will create the directory, if it does not exist.
     */
    bool isAccessible();
    
    /**
     * Returns true, if the directory is RWX able.
     * 
     * Will create the directory, if it does not exist.
     */
    bool isWriteable();
    
    /**
     * Returns a copy of the directory path member
     */
    fs::path getDirectory() const;
    
    /**
     * Will search within this directory for files, matching the given p_phrase
     * 
     * If you like to search in subfolders too, increase p_recursion_level to the level
     * of subfolders, which should be scanned too. Default is 0 (Zero), so we won't search
     * in subfolders.
     */
    bool findFiles( std::vector< fs::path >& p_storage,
                    const std::string& p_phrase,
                    ushort p_mode = LOOKUP_BY_FULLMATCH,
                    ushort p_type = FILE_TYPE_REGULAR,
                    uint p_recursion_level = 0 );
  private:
    fs::path c_directory;
    
    /**
     * -1 => directory access wasn't tested yet (via createIfNotExists)
     *  0 => everything is alright
     *  1 => directory could not be created
     *  4 => the path exists, but is not a directory
     *  6 => the directory is not read/write accessible
     */
    int c_check_cache;
    
  };
}} // cloudos::system

#endif
