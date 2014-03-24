/*
 *   libcloudos
 *   Copyright (C) 2014  Tony Wolf <wolf@os-forge.net>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; version 2 of the License.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/



#include <cloudos/system/StepExecutor.hpp>

namespace cloudos {
namespace system {
  
  StepExecutor::StepExecutor() : c_callback(0) { }
  
  StepExecutor::StepExecutor ( std::string && p_index,
                                               boost::function< bool ()> && p_callback, std::string && p_desc ) : c_callback(0) {
    setIndexName( std::move(p_index) );
    setCallback( std::move(p_callback) );
    setDescription( std::move(p_desc) );
  }
  /*
  StepExecutor::StepExecutor ( StepExecutor && p_from ) {
    c_desc       = std::move( p_from.c_desc       );
    c_callback   = std::move( p_from.c_callback   );
    c_index_name = std::move( p_from.c_index_name );
    c_is_valid   = std::move( p_from.c_is_valid   );
  }
  
  StepExecutor& StepExecutor::operator= ( StepExecutor && p_from ) {
    c_desc       = std::move( p_from.c_desc       );
    c_callback   = std::move( p_from.c_callback   );
    c_index_name = std::move( p_from.c_index_name );
    c_is_valid   = std::move( p_from.c_is_valid   );
    return *this;
  }*/

  void StepExecutor::setCallback ( boost::function< bool ()>&& p_callback ) {
    c_callback = std::move(p_callback);
    checkIsValid();
  }

  void StepExecutor::setDescription ( std::string && p_desc ) {
    c_desc = std::move(p_desc);
  }

  bool StepExecutor::run() {
    LOG_I() << "running step " << getIndexName() << "(" << c_desc << ")";
    if( c_is_valid == false ) {
      LOG_E() << "could not run step, as object is invalid";
      return false;
    }
    
    if( c_callback() == false ) {
      LOG_E() << "step failed";
      return false;
    }
    
    LOG_I() << "finished successful";
    return true;
  }
  
  // 
  // P R I V A T E
  // 
  void StepExecutor::checkIsValid() {
    c_is_valid = getIndexName().empty() == false && c_callback;
  }
  
}}
