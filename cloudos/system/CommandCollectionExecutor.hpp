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



#ifndef CLOUDOS_SYSTEM_COMMAND_COLLECTION_EXECUTOR_HPP__
#define CLOUDOS_SYSTEM_COMMAND_COLLECTION_EXECUTOR_HPP__

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/container/flat_map.hpp>

#include <cloudos/core/Object.hpp>
#include <cloudos/system/Command.hpp>

namespace ps = boost::process;

namespace cloudos {
namespace system {
  
  class CommandCollectionExecutor;
  typedef boost::shared_ptr<CommandCollectionExecutor> CommandCollectionExecutorPointer;
  
  /**
   * Our CommandCollection storage
   * We could not use a map, as we need a specifyable order, while executing all commands
   * std::pair.first should be a description of the CommandVector containment.
   */
  typedef std::pair<std::string, CommandVector>         CommandIndexPair;
  typedef std::vector< CommandIndexPair >              CommandCollection;
  typedef boost::shared_ptr<CommandCollection > CommandCollectionPointer;
  
  typedef std::vector< std::pair<std::string, Command> > CommandErrorVector;
  
  enum CommandCollectionErrorHandlingType {
    CMD_COLLECTION_FAIL_EXEC_ON_ANY_ERROR = 0,
    CMD_COLLECTION_IGNORE_ERRORS_ON_RUN     = 1,  // will ignore errors while executing, but will report them and allSuccess() will fail if a command fails
    CMD_COLLECTION_HIDE_ERRORS_COMPLETELY   = 2   // won't report errors, even when they occure, allSuccess() will always be true, until run() was called
  };
  
  /**
   * Why we need this class?
   * This class will execute all commands within the givven CommandCollection. It will handle logging
   * and the correct form of executing all commands.
   * It will catch errors returned by the command, while respecting the wanted error handling.
   * 
   * What is possible with this class?
   * Execute and control all commands within CommandCollection.
   * 
   * Is there a special behaviour, on special circumstances?
   * None known
   * 
   * How to use this class as an object?
   * CommandCollectionExecutor executor;
   * executor.slotAddonCommandError( &functionToHandleErrorAdditionaly );
   * executor.setErrorHandling( CMD_COLLECTION_STOP_EXEC_ON_FIRST_ERROR );
   * executor.setCommandCollection( cmds );
   * if( executor.isValid() )
   *   executor.run();
   * else
   *   reportErrorInCmdCollection(); // defined by user
   * if( executor.allSuccess() )
   *   party();
   * else
   *   executor.getFailedCommands();
   * 
   */
  class CommandCollectionExecutor : public core::Object {
  public:
    CommandCollectionExecutor();
    
    /**
     * Will set the command error handling type.
     * Default: CMD_COLLECTION_STOP_EXEC_ON_FIRST_ERROR
     */
    void setErrorHandling( CommandCollectionErrorHandlingType p_type = CMD_COLLECTION_FAIL_EXEC_ON_ANY_ERROR );
    
    /**
     * Will set the command collection and will check it.
     * Empty collections are allowed and will result in allSuccess() == true after run() was called.
     * isValid() will be true, if setCommandCollection() succeeded.
     */
    void setCommandCollection( CommandCollection&& p_collection );
    
    /**
     * As we just provide a move() semantic, this is the way to get your data back.
     * But be aware, your commands migth be already executed!
     * Check them for integrity!
     */
    CommandCollection&& getCommandCollection();
    
    /**
     * Will return all failed commands.
     * You migth want to use this, to get the commands, which failed the validation process
     * by setCommandCollection, or which failed while running.
     */
    const CommandErrorVector& getFailedCommands();
    
    /**
     * Will execute all command set by setCommandCollection().
     * Please, check with allSuccess(), if errors occured.
     * Will return true, if isValid() is true and allSuccess() will return true too.
     */
    bool run();
    
  protected:
  private:
    /**
     * Our command collection "flat-tree"
     */
    CommandCollection c_cmd_collection;
    
    /**
     * How to handle errors from command execution
     */
    CommandCollectionErrorHandlingType c_error_handling_type;
    
    /**
     * Will contain the currently running collection index name.
     * If none is running, c_running_collection will be empty.
     */
    std::string c_running_collection;
    
    /**
     * All failed commands will be inserted here...
     */
    CommandErrorVector c_error_cmd_collection;
    
    /**
     * Will check if cmd->isValid() for all commands within c_cmd_collection results in true.
     */
    bool validateCommandCollection();
    
    /**
     * Will start all commands within the given CommandVector::iterator range in parallel
     * TODO
     */
    void startCommands( CommandVector::iterator p_start, CommandVector::iterator p_end );
    
    /**
     * Will wait, until all commands within the given CommandVector::iterator range finished.
     * May retrun false, if a commands return value indicates an error state.
     * TODO
     */
    bool finishCommands( CommandVector::iterator p_start, CommandVector::iterator p_end );
  };

}} // cloudos::system

#endif
