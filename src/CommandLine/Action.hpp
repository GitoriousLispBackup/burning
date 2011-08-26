/*
   Copyright (c)  2011   Dmitry Sopin <sopindm@gmail.com>

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BURNING_COMMANDLINE_ACTION_HPP
#define BURNING_COMMANDLINE_ACTION_HPP
#include <iostream>
#include <map>
#include "Argument.hpp"

namespace burning
{
  namespace commandLine
  {
    /*! An action command line's argument */
    class Action
    { 
    public:
      /*! Constructs action */
      Action( const std::string& name = "", const std::string& description = "" );
     
      /*! Action's name */
      std::string& name();
      /*! Action's name */
      const std::string& name() const;
      /*! Action's description */
      const std::string& description() const;
      
      class CommaInserter;
      class ArgumentsProxy;
      /*! Action's arguments */
      ArgumentsProxy arguments();
      
      /*! Gets argument by name */
      const commandLine::Argument& argument( const std::string& name );
      /*! Gets argument by name */
      const commandLine::Argument& operator[]( const std::string& name );
      
      /*! Gets action argument by name */
      Action& action( const std::string& name );
      
      class PositionalsProxy;
      class PositionalCommaInserter;
      /*! Positional arguments */
      PositionalsProxy positionals();
      
      /*! Gets positional argument by name */
      const commandLine::Argument& positional( const std::string& name );
      
      /*! Checks that this argument is set. */
      bool isSet() const;
      
      class FlagProxy;
      /*! Gets flag presenting help argument*/
      FlagProxy helpFlag();
      /*! Disables help option */
      void disableHelp(); 
      /*! Enables help option */
      void enableHelp();
    
      /*! Prints help message */
      void printHelp( std::ostream& ostream = std::cout );
      
    protected:
      /*! Parses argument in given position */
      bool parsePosition( const std::string& current, commandLine::Iterator& iter );
     
    private:
      bool parseAction( const std::string& name );
      bool parseArgument( const std::string& name, commandLine::Iterator& iterator );
      bool parseShortArgument( char name, commandLine::Iterator& iterator );
       
      void insert( const commandLine::Argument& argument );
      void eraseArgument( const commandLine::Argument& argument );
      
      void insertPositional( const commandLine::Argument& argument );
      
      void insert( const Action& action );
      
      void printArguments( std::ostream& ostream );
      void printActions( std::ostream& ostream );
      void printPositionals( std::ostream& ostream );
      
      typedef std::map< std::string, Action > ActionMap;
      typedef std::map< std::string, commandLine::ArgumentPtr > ArgumentMap;
      typedef std::vector< commandLine::ArgumentPtr > ArgumentVector;
      typedef std::map< char, std::string > ShortNameMap;
      
      ArgumentMap _arguments;
      ArgumentVector _positionals;
      ShortNameMap _shortNames;
      ActionMap _actions;
      std::string _name;
      std::string _description;
      bool _isSet;
      
      size_t _position;
    };
  
    /*! Class presenting arguments of aciton */
    class Action::ArgumentsProxy
    {
    public:
      /*! Constructs proxy */
      ArgumentsProxy( Action& action );
    
      /*! Adds argument to action */
      CommaInserter operator+=( const commandLine::Argument& argument );
      /*! Adds action to action */
      CommaInserter operator+=( const Action& argument );
      
    private:
      Action& _action;
    };
    
    /*! Inserter for comma separated values */
    class Action::CommaInserter
    {
    public:
      /*! Construct inserter */
      CommaInserter( Action& action, const commandLine::Argument& argument );
      /*! Construct inserter */
      CommaInserter( Action& action, const Action& argument );
    
      /*! Adds argument to action */
      CommaInserter& operator,( const commandLine::Argument& argument );
      /*! Adds action to action */
      CommaInserter& operator,( const Action& argument );
    
    private:
      Action& _action;
    };
    
    /*! Representing positional arguments of action */
    class Action::PositionalsProxy
    {
    public:
      /*! Constructs proxy */
      PositionalsProxy( Action& action );
      
      /*! Adds positional argument to action */
      PositionalCommaInserter operator+=( const commandLine::Argument& argument );
      
    private:
      Action& _action;
    };
    
    /*! Inserter for comma separated positional arguments */
    class Action::PositionalCommaInserter
    {
    public:
      /*! Constructs inserter */
      PositionalCommaInserter( Action& action, const commandLine::Argument& argument );
      /*! Constructs inserter */
      PositionalCommaInserter& operator,( const commandLine::Argument& argument );
      
    private:
      Action& _action;
    };
    
    /*! Representing flag argument for action */
    class Action::FlagProxy : public commandLine::Flag
    {
    public:
      /*! Constructs proxy */
      FlagProxy( const commandLine::Flag& flag, Action& action );
      /*! Constructs proxy */
      FlagProxy( const FlagProxy& flag );
      
      /*! Sets flag data from other flag */
      void operator=( const commandLine::Flag& flag );
    
    private:
      Action& _action;
      
    };
  }
}
#endif

