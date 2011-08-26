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

#include <algorithm>
#include "Action.hpp"

using std::vector;
using std::string;
using std::max;
using namespace burning::commandLine;

Action::Action( const std::string& name, const std::string& description ):
                _name( name ),
                _description( description ),
                _isSet( false ),
                _position( 0 )
{
  arguments() += Flag( "help", "Products this help message" );
}

string& Action::name()
{
  return _name;
}

const string& Action::name() const
{
  return _name;
}

const string& Action::description() const
{
  return _description;
}

Action::ArgumentsProxy Action::arguments()
{
  return ArgumentsProxy( *this );
}

const Argument& Action::argument( const string& name )
{
  if( _arguments.count( name ) == 0 )
  {
    LOG( ERROR ) << "Argument with name " << name << " does not exists.";
    exit( EXIT_FAILURE );
  }
  
  return *_arguments[ name ];
}

const Argument& Action::operator[]( const string& name )
{
  if( _arguments.count( name ) != 0 )
    return argument( name );
  return positional( name );
}

Action& Action::action( const string& name )
{
  if( _actions.count( name ) == 0 )
  {
    LOG( ERROR ) << "Action with name " << name << " does not exists.";
    exit( EXIT_FAILURE );
  }
  
  return _actions[ name ];
}

Action::PositionalsProxy Action::positionals()
{
  return PositionalsProxy( *this );
}

const Argument& Action::positional( const string& name )
{
  size_t index( _positionals.size() );
  for( size_t i=0; i<_positionals.size(); i++ )
  {
    if( _positionals[ i ]->name() == name )
    {
      index = i;
      break;
    }
  }
  if( index == _positionals.size() )
  {
    LOG( ERROR ) << "Positional argument with name " << name << " does not exists.";
    exit( EXIT_FAILURE );
  }
  
  return *_positionals[ index ];
}

bool Action::isSet() const
{
  return _isSet;
}

void Action::insert( const Argument& flag )
{
  if( _arguments.count( flag.name() ) != 0 )
  {
    LOG( ERROR ) << "Tried to add command line agrument twice.";
    exit( EXIT_FAILURE );
  }
  if( _shortNames.count( flag.shortName() ) != 0 )
  {
    LOG( ERROR ) << "Tried to set two command line arguments with same short names.";
    exit( EXIT_FAILURE );
  }
  if( _actions.count( "--" + flag.name() ) != 0 )
  {
    LOG( ERROR ) << "Tried to add argument with name same as an action.";
    exit( EXIT_FAILURE );
  }
  if( _actions.count( string( "-" ) + flag.shortName() ) != 0 )
  {
    LOG( ERROR ) << "Tried to add argument with name same as an action.";
    exit( EXIT_FAILURE );
  }
 
  _arguments.insert( std::make_pair( flag.name(), flag.clone() ) );
  if( flag.shortName() != '\0' )
    _shortNames.insert( std::make_pair( flag.shortName(), flag.name() ) );
}

void Action::eraseArgument( const Argument& argument )
{
  _arguments.erase( argument.name() );
  _shortNames.erase( argument.shortName() );
}

void Action::insertPositional( const Argument& argument )
{
  for( size_t i=0; i<_positionals.size(); i++ )
  {
    if( _positionals[ i ]->name() == argument.name() )
    {
      LOG( ERROR ) << "Tried to add positional command line argument twice.";
      exit( EXIT_FAILURE );
    }
  }
  
  _positionals.push_back( argument.clone() );
}

void Action::insert( const Action& action )
{
  if( _actions.count( action.name() ) != 0 )
  {
    LOG( ERROR ) << "Tried to add command line action argument twice.";
    exit( EXIT_FAILURE );
  }
  if( action.name().find( "--" ) == 0 )
  {
    string argName( action.name().substr( 2 ) );
    
    if( _arguments.count( argName ) != 0 )
    {
      LOG( ERROR ) << "Tried to add action with same name as an argument.";
      exit( EXIT_FAILURE );
    }
  }
  if( action.name().find( '-' ) == 0 && action.name().size() == 2 )
  {
    char shortName( action.name()[ 1 ] );
    
    if( _shortNames.count( shortName ) != 0 )
    {
      LOG( ERROR ) << "Tried to add action with same name as an argument.";
      exit( EXIT_FAILURE );
    }
  }
  
  _actions.insert( std::make_pair( action.name(), action ) );
  _actions[ action.name() ].name() = name() + ' ' + action.name();
}

void Action::disableHelp()
{
  eraseArgument( helpFlag() );
}

void Action::enableHelp()
{
  insert( Flag( "help", "Products this help message" ) );
}

Action::FlagProxy Action::helpFlag()
{
  Flag* helpFlag( dynamic_cast< Flag* >( _arguments[ "help" ].get() ) );
  assert( helpFlag != NULL );
  
  return FlagProxy( *helpFlag, *this );
}

void Action::printHelp( std::ostream& ostream )
{
  ostream << "Usage:\n";
  ostream << "  " << _name;
  
  if( _arguments.size() != 0 )
    ostream << " [ARGS]";
  if( _actions.size() != 0 )
    ostream << " [ACTION ...]";
  for( size_t i=0; i<_positionals.size(); i++ )
    ostream << " [" << _positionals[ i ]->name() << ']';
  
  ostream << "\n\n";
  if( _arguments.size() != 0 && _positionals.size() == 0 )
    ostream << "  Where ";
  
  bool haveAdditionalInfo( false );
  if( _positionals.size() != 0 )
  {
    haveAdditionalInfo = true;
    printPositionals( ostream );
  }
  
  if( _arguments.size() != 0 )
  {
    haveAdditionalInfo = true;
    printArguments( ostream );
  }
  if( _actions.size() != 0 )
  {
    if( haveAdditionalInfo )
      ostream << '\n';
    haveAdditionalInfo = true;
    printActions( ostream );
  }
  
  if( haveAdditionalInfo )
    ostream << "\n";
}

void Action::printArguments( std::ostream& ostream )
{
  ostream << "ARGS are:\n";
  
  size_t maxValueLength( 0 );
  std::vector< string > values;
  std::vector< string > descriptions;
  
  std::pair< string, ArgumentPtr > argument;
  BOOST_FOREACH( argument, _arguments )
  {
    string value( "\t--" + argument.first );
    
    bool haveValueArray( argument.second->haveManyValues() );
    bool haveAdditionalInfo( haveValueArray );
    if( argument.second->shortName() != '\0' )
    {
      value += ",-";
      value += argument.second->shortName();
      
      haveAdditionalInfo = true;
    }
    
    if( haveValueArray )
      value += " [PARAM1 ...]";
    else if( argument.second->haveAnyValue() )
    {
      value += " PARAM";
      haveAdditionalInfo = true;
    }
    
    values.push_back( value );
    if( value.size() > maxValueLength )
      maxValueLength = value.size();
    descriptions.push_back( argument.second->description() );
  }
  maxValueLength = max( ( ( maxValueLength + 6 ) / 7 ) * 7, size_t( 21 ) );
  
  for( size_t i=0; i<values.size(); i++ )
  {
    ostream << values[ i ];
    for( size_t j=values[ i ].size(); j<maxValueLength; j++ )
      ostream << ' ';
    
    ostream << '\t';
    ostream << descriptions[ i ] << '\n';
  }
}

void Action::printPositionals( std::ostream& ostream )
{
  size_t maxValueLength( 0 );
  std::vector< string > values;
  std::vector< string > descriptions;
  
  BOOST_FOREACH( ArgumentPtr argument, _positionals )
  {
    string value( argument->name() );
    
    values.push_back( value );
    if( value.size() >= maxValueLength )
      maxValueLength = value.size()+1;
    descriptions.push_back( argument->description() );
  }
  maxValueLength = max( ( ( maxValueLength + 6 ) / 7 ) * 7, size_t( 21 ) );
  
  for( size_t i=0; i<values.size(); i++ )
  {
    if( i == 0 )
      ostream << "  Where ";
    else
      ostream << '\t';
    
    ostream << values[ i ];
    for( size_t j=values[ i ].size(); j<maxValueLength; j++ )
      ostream << ' ';
    
    ostream << '\t';
    ostream << descriptions[ i ] << '\n';
  }
  
  ostream << "\n        ";
}

void Action::printActions( std::ostream& ostream )
{
  ostream << "        ACTION is one of:\n";
  
  vector< string > names;
  vector< string > descriptions;
  size_t maxNameLength( 0 );
  
  std::pair< string, Action > action;
  BOOST_FOREACH( action, _actions )
  {
    names.push_back( action.first );
    if( action.first.size() > maxNameLength )
      maxNameLength = action.first.size();
    descriptions.push_back( action.second.description() );
  }
  
  maxNameLength = max( ( ( maxNameLength + 6 ) / 7 )* 7, size_t( 21 ) );
  
  for( size_t i=0; i<names.size(); i++ )
  {
    ostream << '\t' << names[ i ];
    for( size_t j=names[ i ].size(); j<maxNameLength; j++ )
      ostream << ' ';
    ostream << '\t' << descriptions[ i ];
    ostream << '\n';
  }
    
  ostream << "\n  For help on concrete action\ttype: " << _name << " ACTION --help\n";
}

bool Action::parsePosition( const std::string& arg, Iterator& iter )
{
  if( parseAction( arg ) )
    return true;
  
  for( ActionMap::iterator it = _actions.begin(); it != _actions.end(); it++ )
  {
    if( !it->second.isSet() )
      continue;
    
    if( it->second.parsePosition( arg, iter ) )
      return true;
  }
    
  if( arg.find( "--" ) == 0 )
    return parseArgument( arg.substr( 2 ), iter );
  else if( arg.find( '-' ) == 0 )
  {
    string shortNames( arg.substr( 1 ) );
    BOOST_FOREACH( char shortName, shortNames )
    {
      if( !parseShortArgument( shortName , iter ) )
	return false;
    }
      
    return true;
  }
  
  if( _position != _positionals.size() )
  {
    iter--;
    _positionals[ _position ]->parse( iter );
    _position++;
    
    return true;
  }
  
  return false;
}

bool Action::parseAction( const std::string& name )
{
  if( _actions.count( name ) == 0 )
    return false;
  
  if( _actions[ name ]._isSet )
    return false;
  
  _actions[ name ]._isSet = true;
  return true;
}

bool Action::parseArgument( const std::string& name, Iterator& iter )
{
  if( _arguments.count( name ) == 0 )
    return false;
  
  _arguments[ name ]->parse( iter );
  return true;
}

bool Action::parseShortArgument( char shortName, Iterator& iter )
{
  if( _shortNames.count( shortName ) == 0 )
    return false;
  
  return parseArgument( _shortNames[ shortName ], iter );
}
/*
 * Agruments proxy
 */

Action::ArgumentsProxy::ArgumentsProxy( Action& action ):
                                        _action( action )
{
}

Action::CommaInserter Action::ArgumentsProxy::operator+=( const Argument& argument )
{
  return CommaInserter( _action, argument );
}

Action::CommaInserter Action::ArgumentsProxy::operator+=( const Action& argument )
{
  return CommaInserter( _action, argument );
}

/*
 *  CommaInserter
 */

Action::CommaInserter::CommaInserter( Action& action , const Argument& arg ):
                                           _action( action )
{
  _action.insert( arg );
}

Action::CommaInserter::CommaInserter( Action& action , const Action& arg ):
                                           _action( action )
{
  _action.insert( arg );
}

Action::CommaInserter& Action::CommaInserter::operator,( const Argument& arg )
{
  _action.insert( arg );
  
  return *this;
}

Action::CommaInserter& Action::CommaInserter::operator,( const Action& arg )
{
  _action.insert( arg );
  
  return *this;
}

/*
 * Positionals proxy
 */

Action::PositionalsProxy::PositionalsProxy( Action& action ):
                                            _action( action )
{
}

Action::PositionalCommaInserter Action::PositionalsProxy::operator+=( const commandLine::Argument& argument )
{
  return PositionalCommaInserter( _action, argument );
}

/*
 * Positional comma inserter
 */

Action::PositionalCommaInserter::PositionalCommaInserter( Action& action, const commandLine::Argument& argument ): 
                                                          _action( action )
{
  _action.insertPositional( argument );
}

Action::PositionalCommaInserter& Action::PositionalCommaInserter::operator,( const commandLine::Argument& argument )
{
  _action.insertPositional( argument );
  return *this;
}

/*
 * Flag proxy
 */

Action::FlagProxy::FlagProxy( const Flag& flag, Action& action ):
                              Flag( flag ),
                              _action( action )
{
}

void Action::FlagProxy::operator=( const Flag& flag )
{
  _action.eraseArgument( flag );
  Flag::operator=( flag );
  _action.insert( flag );
}
