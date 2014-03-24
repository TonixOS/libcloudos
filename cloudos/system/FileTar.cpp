
#include <cloudos/system/FileTar.hpp>
#include <cloudos/system/Command.hpp>

namespace cloudos {
namespace system {
  
  FileTar::FileTar ( fs::path && p_file ) : File( std::move( p_file ) ) {
    setRequireExists(true);
  }
  
  FileTar::FileTar ( const fs::path& p_file ) : File ( p_file ) {
    setRequireExists(true);
  }
  
  FileTar::FileTar ( fs::path&& p_file, Directory&& p_destination_dir ) : File ( std::move(p_file) ) {
    setRequireExists(true);
    setDirectory( std::move(p_destination_dir) );
  }
  
  FileTar::FileTar ( const FileTar& p_from_file ) {
    c_is_valid        = p_from_file.c_is_valid;
    c_is_unpacked     = p_from_file.c_is_unpacked;
    c_destination_dir = p_from_file.c_destination_dir;
    c_file            = p_from_file.c_file;
    c_file_str        = p_from_file.c_file_str;
  }
  
  FileTar& FileTar::operator= ( FileTar&& p_from_file ) {
    c_is_valid        = std::move(p_from_file.c_is_valid);
    c_is_unpacked     = std::move(p_from_file.c_is_unpacked);
    c_destination_dir = std::move(p_from_file.c_destination_dir);
    c_file            = std::move(p_from_file.c_file);
    c_file_str        = std::move(p_from_file.c_file_str);
    return *this;
  }
  
  short int FileTar::unpack() {
    if( c_is_unpacked ) {
      LOG_T() << "won't do anything, as tar file is already unpacked!";
      return 0; // we already unpacked the tar file
    }
    
    LOG_I() << "trying to unpack file " << c_file_str << " to: " << c_destination_dir.getDirectory();
    if( reportInvalidObjectError( __PRETTY_FUNCTION__ ) ) {
      return 1;
    }
    
    Command cmd;
    cmd << "tar" << "-xf" << c_file_str << "--directory=" + c_destination_dir.getDirectory().string();
    
    if( cmd.waitUntilFinished() != 0 ) {
      LOG_E() << "unpack tar file failed!";
      return 3;
    }
    
    c_is_unpacked = true;
    
    return 0;
  }
  
  short int FileTar::pack() {
    // TODO
    LOG_F() << "this function is not implemented";
    return 0;
  }
  
  bool FileTar::isUnpacked() const {
    return c_is_unpacked;
  }
  
  bool FileTar::setDirectory( Directory&& p_destination_dir ) {
    if( c_destination_dir.getDirectory() == p_destination_dir.getDirectory() ) {
      // do nothing...
      LOG_I() << "trying to set the same tar file destination twice... do nothing...";
      return true;
    }
    
    if( p_destination_dir.isWriteable() == false ) {
      LOG_E() << "tar directory (" << p_destination_dir.getDirectory().string() << ") is not writeable! Abort!";
      return false;
    }
    c_destination_dir = std::move(p_destination_dir);
    checkIsValid();
    c_is_unpacked = false; // we got a new destination...
    return true;
  }
  
  // 
  // P R O T E C T E D
  // 
  void FileTar::checkIsValid() {
    cloudos::system::File::checkIsValid();
    if( c_is_valid == false ) {
      return;
    }
    
    // set to false, as we have some more requirements...
    c_is_valid = false;
    
    if( c_destination_dir.isValid() == false ) {
      return;
    }
    
    // we need to check the file extension too
    std::vector<std::string> validExtensions;
    validExtensions.push_back( std::move(".tar")     );
    validExtensions.push_back( std::move(".tgz")     );
    validExtensions.push_back( std::move(".tar.gz")  );
    validExtensions.push_back( std::move(".tar.xz")  );
    validExtensions.push_back( std::move(".tar.bz2") );
    
    for(auto& line : validExtensions) {
      if( filenameEndsWith( std::move(line) ) ) {
        c_is_valid = true;
        return;
      }
    }
  }
  
  // 
  // P R I V A T E
  // 
  
}}
