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



#include <cloudos/system/CommandCollectionExecutor.hpp>

namespace cloudos {
namespace system {
  
  CommandCollectionExecutor::CommandCollectionExecutor() {
    c_cmd_collection = CommandCollection();
    c_error_cmd_collection = CommandErrorVector();
    setErrorHandling();
    c_running_collection = std::string();
  }

  CommandCollection && CommandCollectionExecutor::getCommandCollection() {
    c_is_valid = false;
    return std::move( c_cmd_collection );
  }
  
  const CommandErrorVector& CommandCollectionExecutor::getFailedCommands() {
    return c_error_cmd_collection;
  }

  bool CommandCollectionExecutor::run() {
    if( c_is_valid == false ) {
      return false;
    }
    
    CommandCollection::iterator i_seq = c_cmd_collection.begin(),
                                i_seq_end = c_cmd_collection.end();
    
    CommandVector::iterator vstart, vend;
    for(; i_seq != i_seq_end; ++i_seq) {
      c_running_collection = i_seq->first;
      vstart = i_seq->second.begin();
      vend   = i_seq->second.end();
      
      startCommands( vstart, vend );
      
      if( finishCommands( vstart, vend ) == false &&
          c_error_handling_type == CMD_COLLECTION_FAIL_EXEC_ON_ANY_ERROR ) {
        return false;
      }
    }
    
    return true;
  }

  void CommandCollectionExecutor::setCommandCollection ( CommandCollection && p_collection ) {
    c_error_cmd_collection.clear();
    LOG_I() << "set command collection with " << p_collection.size() << " set of commands";
    c_cmd_collection = std::move( p_collection );
    c_is_valid = validateCommandCollection();
  }

  void CommandCollectionExecutor::setErrorHandling ( CommandCollectionErrorHandlingType p_type ) {
    c_error_handling_type = p_type;
  }
  
  // 
  // P R I V A T E
  // 
  
  bool CommandCollectionExecutor::validateCommandCollection() {
    CommandCollection::iterator i_seq = c_cmd_collection.begin(),
                                i_seq_end = c_cmd_collection.end();
    
    CommandVector::iterator i_vec, i_vec_end;
    for( ; i_seq != i_seq_end; ++i_seq ) {
      
      for( i_vec = i_seq->second.begin(), i_vec_end = i_seq->second.end(); i_vec != i_vec_end; ++i_vec) {
        if( i_vec->isValid() == false ) {
          LOG_E() << "found invalid command (" << i_vec->getCommandName() << ") in collection " << c_running_collection;
          c_error_cmd_collection.push_back( std::pair<std::string, Command>( c_running_collection, *i_vec ) );
        }
      }// iterate over vector
      
    }// iterate over map
    
    return c_error_cmd_collection.empty();
  }
  
  void CommandCollectionExecutor::startCommands ( CommandVector::iterator p_start, CommandVector::iterator p_end ) {
    //FIXME report starting commands
    for(; p_start != p_end; ++p_start) {
      if( p_start->start() == false ) {
        c_error_cmd_collection.push_back( std::pair<std::string, Command>( c_running_collection, *p_start ) );
      }
    }
  }
  
  bool CommandCollectionExecutor::finishCommands ( CommandVector::iterator p_start, CommandVector::iterator p_end ) {
    //FIXME report finishing commands...
    int return_value = 0;
    bool exec_success = true;
    for(; p_start != p_end; ++p_start) {
      //TODO check return types and add logging/stats
      return_value = p_start->waitUntilFinished();
      if( return_value != 0 && c_error_handling_type != CMD_COLLECTION_HIDE_ERRORS_COMPLETELY ) {
        
        // add command to failed reporting
        c_error_cmd_collection.push_back( std::pair<std::string, Command>( c_running_collection, *p_start ) );
        
        // set a new state, just, if exec_success isn't already in state false
        if( exec_success && c_error_handling_type == CMD_COLLECTION_FAIL_EXEC_ON_ANY_ERROR ) {
          exec_success = false;
        }
      }
    }
    return exec_success;
  }
  
}}
