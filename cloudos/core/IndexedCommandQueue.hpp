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



#ifndef CLOUDOS_CORE_INDEXED_QUEUE_HPP__
#define CLOUDOS_CORE_INDEXED_QUEUE_HPP__


/**
 * DEPRECATED
 * 
 * Please use CommandCollectionExecutor instead!
 * 
 */

#include <string>

#include <boost/container/flat_map.hpp>
#include <boost/container/stable_vector.hpp>

#include <cloudos/core/Object.hpp>
#include <cloudos/system/Command.hpp>

namespace bc = boost::container;

namespace cloudos {
namespace core {
  
  enum InsertType {
    INSERT_FRONT = 0,
    INSERT_APPEND = 1
  };
  
  // for more easily reading
  typedef bc::flat_map<std::string, bc::stable_vector<system::Command>::const_iterator > IndexedQueueMap;
  
  /**
   * Why we need this class?
   * If you like to create a queue but need to be able to insert elements, based on an givven index.
   * All data is ordered by insertion. The container won't order by itself. Use the index to oder elements.
   * 
   * What is possible with this class?
   * Create and use an indexed queue, wich is based on an stable_vector and an flat_multimap
   * 
   * Is there a special behaviour, on special circumstances?
   *   - moveable (see std::move())
   * 
   * How to use this class as an object?
   * IndexedQueue<system::Command> commandQueue;
   * system::Command cmd;
   * cmd.setIndexName("middle"); // required by commandQueue
   * commandQueue.insert(std::move(cmd)); // commandQueue.insert(cmd) is supported too
   * system::Command cmd2;
   * commandQueue.insertAfter("middle", std::move(cmd2));
   */
  class IndexedCommandQueue {
  public:
    IndexedCommandQueue();
    
    /**
     * If you insert a command withour an index name, you won't be able to remove this command
     * later on!
     * 
     * We don't support multiple index names! If the index is already in use, the existing one will
     * be overwritten by the given one!
     * 
     * By choosing the insert-type "INSERT_FRONT", the function will ignore p_after
     */
    bool insert(system::Command&& p_cmd,
                std::string&& p_after = std::move(std::string()),
                InsertType p_type = INSERT_APPEND);
    
    /**
     * does the same as insert(system::Command&&), prefer it - this one is for compatibility only
     */
    bool insert(const system::Command& p_cmd,
                const std::string& p_after = std::string(),
                InsertType p_type = INSERT_APPEND);
    
    /**
     * Removes a item from the queue, by key.
     * Returns true, if the item is found and removed, else false.
     */
    bool remove(std::string&& p_indexName);
    
    /**
     * does the same as remove(std::string&&), prefer it - this one is for compatibility only
     */
    inline bool remove(const std::string& p_indexName);
    
    /**
     * checks, if the givven key (index) is found.
     * If found, returns the const iterator of this element, else 
     */
    bool exists( const std::string& p_indexName ) const;
    
    /**
     * Will start all containing system::Command'
     */
    bool run();
    
  protected:
  private:
    /**
     * Our command list/queue storage
     */
    bc::stable_vector<system::Command> c_queue;
    
    /**
     * Our command index
     */
    IndexedQueueMap c_index;
  };
}}

#endif
