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



#ifndef CLOUDOS_CORE_OBJECT_HPP__
#define CLOUDOS_CORE_OBJECT_HPP__

#include <cloudos/core/logging.hpp>

namespace cloudos {
namespace core {
  
  /**
   * Why we need this class?
   * This will add a few special, generic object variables/functions
   * Esspecialy for debugging...
   * 
   * What is possible with this class?
   * Just inherent this class
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * Don't do this
   */
  class Object : public Logger {
  public:
    
    virtual bool isValid() const { return c_is_valid; }
    
  protected:
    Object();
    ~Object();
    
    bool c_is_valid;
    
    /**
     * On default, this function will do nothing...
     * 
     * But it should be used to set c_is_valid.
     */
    virtual void checkIsValid() {}
    
    virtual bool reportInvalidObjectError( const std::string& p_from_function ) const;
  private:
  };
  
}} // cloudos::core

#endif
