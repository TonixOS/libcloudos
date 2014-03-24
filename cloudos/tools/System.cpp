
// for backtrace
extern "C" {
#include <execinfo.h>
#include <signal.h>

// access to local disks
#include <parted/device.h>
}

#include <boost/tokenizer.hpp>

#include <cloudos/core/logging.hpp>
#include <cloudos/system/HDDisk.hpp>

#include "System.hpp"

namespace cloudos {
namespace tools {
  
  void System::splitString ( const std::string& p_source_string,
                             const std::string& p_delimiters,
                             std::vector<std::string>& p_storage ) {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    
    boost::char_separator<char> sep(p_delimiters.c_str(), std::string("").c_str(), boost::keep_empty_tokens);
    tokenizer fields(p_source_string, sep);
    for(tokenizer::const_iterator it = fields.begin(); it != fields.end(); ++it) {
      p_storage.push_back( *it );
    }
  }
  
  System::NICConfigPointerVector System::getAvailableInterfaces() {
    struct if_nameindex *if_nics, *nic;
    NICConfigPointerVector ifaces;
    
    if_nics = if_nameindex();
    if(if_nics == nullptr) {
      return std::move(ifaces); // return empty ifaces
    }
    
    std::string lo("lo"), bond("bond0");
    for (nic = if_nics; nic->if_index != 0 && nic->if_name != nullptr; ++nic) {
      if( lo.compare( nic->if_name ) != 0 && bond.compare( nic->if_name ) != 0 ) {
        tools::NetworkInterface::ConfigPointer iface( new config::os::NetworkInterface() );
        iface->set_index( nic->if_name );
        ifaces.push_back( std::move(iface) );
      }
    }
    return std::move(ifaces);
  }
  
  System::HDDiskConfigPointerVector System::getAvailableHDDisks ( bool p_filter_installable ) {
    core::Logger::LogSourceType lg;
    BOOST_LOG_SEV(lg, blog::trivial::info) << "detecting harddisks...";
    
    HDDiskConfigPointerVector disk_vector;
    
    // scan for disks
    ped_device_probe_all();
    // 0 will indicate, that we need the first element, instead of "next"
    PedDevice *device = ped_device_get_next(0);
    for( ; device; device = ped_device_get_next(device) ) {
      pb::uint64 disk_size = system::HDDisk::calculateSize(device->hw_geom, device->sector_size);
      if( p_filter_installable && disk_size < 20 ) {
        BOOST_LOG_SEV(lg, blog::trivial::info) << "ignore disk " << device->path << " because it's size is lower 20GB... got disk_size: " << disk_size << "GB";
        
        continue; // ignore disks where their size is lower 20GB...
      }
      
      BOOST_LOG_SEV(lg, blog::trivial::info) << "found disk " << device->path
                                             << " with size: " << disk_size << "GB"
                                             << " and model: " << device->model;
      boost::shared_ptr<config::os::hw::HDDisk> disk( new config::os::hw::HDDisk() );
      disk->set_index( device->path );
      disk->set_size( disk_size );
      disk->set_model( device->model );
      disk_vector.push_back( disk );
    }
    return std::move( disk_vector );
  }

  void System::enableBacktrace() {
    // register backtrace handler
    signal(SIGSEGV, System::backtrace);
  }
  
  void System::backtrace(int p_sig) {
    core::Logger::LogSourceType lg;
    
    char** backtrace_lines;
    void *backtrace_elements[20];
    size_t available_stack_elements;
    
    BOOST_LOG_SEV(lg, blog::trivial::fatal) << "Program got SIGNAL:..." << p_sig;
    BOOST_LOG_SEV(lg, blog::trivial::fatal) << "running backtrace:";
    
    // get void*'s for up to 20 entries on the stack
    // size will have the real number of entries, stored in array
    // we need to reference from start (with the leading ::), as this function is also called "backtrace()"
    available_stack_elements = ::backtrace(backtrace_elements, 20);
    
    // print out all the frames to LOG
    backtrace_lines = backtrace_symbols(backtrace_elements, available_stack_elements);
    
    for(int i = 0; i < available_stack_elements; ++i) {
      BOOST_LOG_SEV(lg, blog::trivial::fatal) << backtrace_lines[i];
    }
    
    exit( p_sig );
  }
  
}
}
