
#ifndef CLOUDOS_SYSTEM_FILE_TAR_HPP__
#define CLOUDOS_SYSTEM_FILE_TAR_HPP__

#include <boost/smart_ptr/shared_ptr.hpp>

#include <cloudos/system/File.hpp>
#include <cloudos/system/Directory.hpp>

namespace cloudos {
namespace system {
  
  class FileTar;
  typedef boost::shared_ptr<FileTar> FileTarPointer;
  
  /**
   * Why we need this class?
   * To (un)pack tar files to/from a directory.
   * 
   * What is possible with this class?
   * Extract files from a tar file or put files to a tar file
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * FileTar tar("file_name.tar.gz");
   * tar.setDirectory("/to/location/");
   * tar.unpack();
   */
  class FileTar : public File {
  public:
    FileTar ( fs::path&& p_file );
    FileTar ( const fs::path& p_file );
    FileTar ( fs::path&& p_file, Directory&& p_destination_dir );
    FileTar( const FileTar& p_from_file );
    
    /**
     * for base init; but you need to drop this object, as you are unable to set it in a valid state!
     * Use std::move() to init your FileTar object!
     * 
     * Like:
     * FileTar tar; // always keeping invalid state
     * FileTar tar_valid("/path/to/file.tar"); // valid
     * tar = std::move(tar_valid); // tar is now tar_valid, so tar is valid! (tar_valid migth be invalid now)
     */
    FileTar() {}
    
    FileTar& operator=(FileTar&& p_from_file);
    
    /**
     * Unpacks a tar file to p_destination_dir
     * 
     * Returns
     *   0 => on success
     *   1 => if object is not valid
     *   3 => if unpacking failed
     */
    short unpack();
    
    short pack();
    
    bool isUnpacked() const;
    
    /**
     * Will return true, if we were able to set the destination or source directory.
     * 
     * else, will return false.
     */
    bool setDirectory( Directory&& p_destination_dir );
    
  protected:
    virtual void checkIsValid();
    
  private:
    bool c_is_unpacked;
    
    /**
     * Our destination directory, where the data will be stored
     */
    Directory c_destination_dir;
  };
}} // cloudos::system

#endif
