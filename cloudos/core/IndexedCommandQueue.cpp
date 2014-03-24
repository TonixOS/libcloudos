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



#include <cloudos/core/IndexedCommandQueue.hpp>

// DEPRECATED see .hpp

namespace cloudos {
namespace core {

  IndexedCommandQueue::IndexedCommandQueue() {

  }

  bool IndexedCommandQueue::insert ( system::Command && p_cmd, std::string && p_after, InsertType p_type ) {
    std::string index_name( p_cmd.getIndexName() );
    bc::stable_vector<system::Command>::const_iterator insert_at;
    
    /**
     * If we need to insert in FRONT mode, ignore p_after and insert at cbegin().
     * Else, try to find p_after in our index, if found, insert after the found element.
     * If not found, insert at the end...
     */
    if( p_type == INSERT_FRONT ) {
      insert_at = c_queue.cbegin();
    } else {
      if( p_after.empty() == false ) {
        IndexedQueueMap::const_iterator map_it = c_index.find( p_after );
        if( map_it == c_index.cend() ) {
          insert_at = c_queue.cend();
        } else {
          insert_at = map_it->second;
          ++insert_at;
        }
      } else {
        insert_at = c_queue.cend();
      }
    }
    
    // now insert element
    if( index_name.empty() ) {
      c_queue.insert( insert_at, std::move(p_cmd) );
    } else {
      c_index[ index_name ] = c_queue.insert( insert_at, std::move(p_cmd) );
    }

    return true;
  }

  bool IndexedCommandQueue::insert ( const system::Command& p_cmd, const std::string& p_after, InsertType p_type ) {
    system::Command cmd = p_cmd;
    std::string after = p_after;
    return insert( std::move(cmd), std::move(after), p_type );
  }

  bool IndexedCommandQueue::remove ( std::string && p_indexName ) {
    IndexedQueueMap::iterator remove_it;
    remove_it = c_index.find( std::move(p_indexName) );
    if( remove_it == c_index.end() ) {
      return false;
    }
    c_queue.erase( remove_it->second );
    c_index.erase( remove_it );
    return true;
  }

  inline bool IndexedCommandQueue::remove ( const std::string& p_indexName ) {
    return remove( std::move(p_indexName) );
  }

  bool IndexedCommandQueue::exists ( const std::string& p_indexName ) const {
    return c_index.find( std::move(p_indexName) ) != c_index.end();
  }
  
  bool IndexedCommandQueue::run() {
    bc::stable_vector<system::Command>::iterator it, end;
    it = c_queue.begin();
    end = c_queue.end();
    for(; it != end; ++it ) {
      it->waitUntilFinished();
    }
    return true;
  }

  
}}
