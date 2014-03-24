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



#ifndef CLOUDOS_CORE_SHARE_ABLE_HPP__
#define CLOUDOS_CORE_SHARE_ABLE_HPP__


namespace cloudos {
namespace core {
  
  /**
   * Why we need this class?
   * This class provides you with all functionality, required by an shared object.
   * You are not allowed to instanciate this class directly!
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * class YourClass : public ShareAble { ... };
   * 
   */
  class ShareAble {
  public:
  protected:
    ShareAble() {
      c_instance_counter = 0;
    }
    inline void incrementInstanceCount() {
      ++c_instance_counter;
    }
    inline void decrementInstanceCount() {
      if(c_instance_counter > 0) {
        --c_instance_counter;
      }
    }
    inline uint getInstanceCount() {
      return c_instance_counter;
    }
  private:
    uint c_instance_counter;
  };
}}
#endif
