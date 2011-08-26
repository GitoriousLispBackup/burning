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

#include <time.h>
#include "Table.hpp"
#include "Profile.hpp"

using std::string;
using std::vector;
using namespace burning;
using namespace burning::profiling;

Profile::Profile() : _rootPhase( new Phase() ), _current()
{
  _current.push( _rootPhase.get() );
  
  _rootPhase->beginIteration( "" );
}

void Profile::addValue( const std::string& name, const Value& value )
{
  _current.top()->addValue( name, value );
}

void Profile::beginPhase( const std::string& name )
{
  beginLoop( name );
  _current.top()->beginIteration( "" );
}

void Profile::endPhase( TimeMeasure measure )
{
  _current.top()->endIteration( measure );
  endLoop();
}

Profile& Profile::global()
{
  static Profile profile;
  return profile;
}

void Profile::beginIteration( const xml::Attribute::ValueType& name )
{
  _current.top()->beginIteration( xml::Attribute::ValueType( name ) );
}
    
void Profile::endIteration( TimeMeasure measure )
{
  _current.top()->endIteration( measure );
}

void Profile::beginLoop( const string& name )
{
  PhasePtr newPhase( new Phase() );
  _current.top()->addPhase( name, newPhase );
  
  _current.push( newPhase.get() );
}

void Profile::endLoop()
{
  if( !_current.top()->finished() )
  {
    LOG( INFO ) << "Iteration's end missing.";
    exit( EXIT_FAILURE );
  }
  _current.pop();
  
  if( _current.empty() )
  {
    LOG( ERROR ) << "Phase's begin missing.";
    exit( EXIT_FAILURE );
  }
}

xml::NodePtr Profile::toXml()
{
  if( _current.size() > 1 )
  {
    LOG( INFO ) << _current.size();
    LOG( ERROR ) << "Tried to write uncomplete profile.";
    exit( EXIT_FAILURE );
  }
  
  if( !_rootPhase->finished() )
    _rootPhase->endIteration();
  
  xml::NodePtr ret( _rootPhase->toXml() );
  ret->name() = "profile";
  
  return ret;
}

ProfilePtr Profile::fromXml( xml::Node& node )
{
  if( node.name() != "profile" )
  {
    LOG( ERROR ) << "Cannot create profile from tag " << node.name();
    return ProfilePtr();
  }
  string name( node.name() );
  node.name() = "phase";
  node.attr( "name" ) = "";
  
  PhasePtr root( Phase::fromXml( node ) );
  node.name() = name;
  
  ProfilePtr ret( new Profile() );
  ret->_rootPhase = root;
  
  return ret;
}

bool Profile::preparePrint( Table* table, PhasePtr& loopRoot, bool printed )
{
  string rootName( "" );
  bool isLast = false;
  for(;;)
  {
    if( printed )
      printed = false;
    else
    {
      if( loopRoot->iterations().size() > 1 )
      {
        isLast = true;
        break;
      }
      if( loopRoot->values().size()>1 )
        break;
      if( loopRoot->phases().size() > 1 )
        break;
    }
    
    rootName = loopRoot->phases().begin()->first;
    loopRoot = loopRoot->phases().begin()->second[ 0 ];
  }
  
  loopRoot->toTable( table );
  
  if( loopRoot->isLoop() )
    table->column( 0 ).name() = rootName;
  
  if( loopRoot->phases().size() == 0 )
    return true;
  
  return isLast;
}

void Profile::print( std::ostream& ostream )
{
  if( !_rootPhase->finished() )
    _rootPhase->endIteration();
  
  PhasePtr loopRoot( _rootPhase );
  bool printed( false );
  for(;;)
  {
    Table table;
    bool isLast = preparePrint( &table, loopRoot, printed );
    table.print( ostream );
    
    if( isLast )
      break;
    
    ostream << std::endl;
    printed = true;
  }
}

void Profile::printHtml( std::ostream& ostream )
{
  if( !_rootPhase->finished() )
    _rootPhase->endIteration();
  
  PhasePtr loopRoot( _rootPhase );
  bool printed( false );
  for(;;)
  {
    Table table;
    bool isLast = preparePrint( &table, loopRoot, printed );
    table.printHtml( ostream );
    
    if( isLast )
      break;
    
    ostream << std::endl;
    printed = true;
  }
}
