
#include <cloudos/core/IndexName.hpp>

namespace cloudos {
namespace core {
  
  void IndexName::setIndexName ( std::string && p_index_name ) {
    c_index_name = std::move(p_index_name);
  }
  
  void IndexName::setIndexName ( const std::string& p_index_name ) {
    c_index_name = p_index_name;
  }
  
}}
