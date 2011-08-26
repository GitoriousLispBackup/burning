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

#include <boost/assign/std/map.hpp>
#include <boost/foreach.hpp>
#include "Table.hpp"
#include "Phase.hpp"

using std::string;
using std::map;
using std::vector;
using namespace boost::assign;
using namespace burning::profiling;

Phase::Phase() : _values(),
                 _phases(),
                 _began( false )
{
}

Phase::Phase( const Phase& phase 
            ) : _values( phase._values ),
		_phases( phase._phases ),
                _beginTime( phase._beginTime ),
                _began( phase._began )
{
}

void Phase::operator=( const Phase& phase )
{
  _values = phase._values;
  _phases = phase._phases;
  _beginTime = phase._beginTime;
  _began = phase._began;
}

const Phase::ValueMap& Phase::values() const
{
  return _values;
}

void Phase::addValue( const string& name, const Value& value )
{
  // Checking that all previous iterations have only one value with same name
  if( _values[ name ].size() < _iterations.size() - 1 && _iterations.size() > 0 ) 
  {
    LOG( ERROR ) << "Value with name " << name << " missed.";
    exit( EXIT_FAILURE );
  }
  if( _iterations.size() <= _values[ name ].size() )
  {
    LOG( ERROR ) << "Tried to add value without related iteration.";
    exit( EXIT_FAILURE );
  }
  _values[ name ].push_back( value );
}

const Phase::PhaseMap& Phase::phases() const
{
  return _phases;
}

void Phase::addPhase( const string& name, const PhasePtr& phase )
{
  if( _phases[ name ].size() < _iterations.size() - 1 )
  {
    LOG( ERROR ) << "Subphase with name " << name << " missed.";
    exit( EXIT_FAILURE );
  }
  if( _phases[ name ].size() >= _iterations.size() )
  {
    LOG( ERROR ) << "Tried to add subphase without related iteration.";
    exit( EXIT_FAILURE );
  }
  _phases[ name ].push_back( phase );
}

void Phase::beginIteration( const xml::Attribute::ValueType& name )
{
  if( _began )
  {
    LOG( ERROR ) << "Cannot start phase twice.";
    exit( EXIT_FAILURE );
  }
  else
    _began = true;
        
  _iterations.push_back( name );
  clock_gettime( CLOCK_MONOTONIC, &_beginTime );
}

void Phase::endIteration( TimeMeasure measure )
{
  timespec endTime;
  clock_gettime( CLOCK_MONOTONIC, &endTime );
  
  if( !_began )
  {
    LOG( ERROR ) << "Tried to end unstarted phase.";
    exit( EXIT_FAILURE );
  }
  else
    _began = false;
  
  long time = endTime.tv_sec - _beginTime.tv_sec;
  time *= measure;
  
  time +=( endTime.tv_nsec - _beginTime.tv_nsec ) / ( nanoseconds / measure );
  
  string measureName;
  switch( measure )
  {
    case seconds:
    {
      measureName = "s";
    }break;
    case milliseconds:
    {
      measureName = "ms";
    }break;
    case microseconds:
    {
      measureName = "mcs";
    }break;
    case nanoseconds:
    {
      measureName = "ns";
    }break;
  }
  
  _values[ "time" ].push_back( Value( time, measureName ) );
}

burning::xml::NodePtr Phase::iterationToXml( size_t index )
{
  assert( index < _iterations.size() );
  
  xml::NodePtr ret( xml::Node::create( "iteration" ) );
  
  std::pair< string, vector< Value > > value;
  BOOST_FOREACH( value, _values )
  {
    if( value.second.size() <= index )
    {
      LOG( ERROR ) << "Value with name " << value.first << " missed in iteration " << _iterations[ index ];
      exit( EXIT_FAILURE );
    }
    
    xml::NodePtr valueNode( xml::Node::create( "value" ) );
    valueNode->attr( "name" ) = value.first;
    valueNode->attr( "value" ) = value.second[ index ].rawValue();
    if( value.second[ index ].measure() != "" )
      valueNode->attr( "measure" ) = value.second[ index ].measure();
    
    ret->childs() += valueNode;
  }
  
  std::pair< string, vector< PhasePtr > > phase;
  BOOST_FOREACH( phase, _phases )
  {
    if( phase.second.size() <= index )
    {
      LOG( ERROR ) << "Phase with name " << phase.first << " missed in iteration " << _iterations[ index ];
      exit( EXIT_FAILURE );
    }
    
    xml::NodePtr subxml( phase.second[ index ]->toXml() );
    subxml->attr( "name" ) = phase.first;
    ret->childs() += subxml;
  }
  
  return ret;
}

burning::xml::NodePtr Phase::toXml()
{
  if( _iterations.size() == 0 )
    return xml::Node::create( "phase" );
  
  if( _iterations.size() == 1 && _iterations[ 0 ] == string( "" ) )
  {
    xml::NodePtr iter( iterationToXml( 0 ) );
    iter->name() = "phase";
    return iter;
  }
  
  xml::NodePtr ret( xml::Node::create( "loop" ) );
  for( size_t i=0; i<_iterations.size(); i++ )
  {
    xml::NodePtr newNode( iterationToXml( i ) );
    newNode->attr( "name" ) = _iterations[ i ];
    
    ret->childs() += newNode;
  } 
  
  return ret;
}

void Phase::setValueFromXml( const xml::NodePtr& node )
{
  assert( node->name() == "value" );
  
  if( !node->attr( "name" ).isSet() )
    LOG( ERROR ) << "Name attribute for value not set.";
  
  if( !node->attr( "value" ).isSet() )
    LOG( ERROR ) << "Value attribute for value not set.";
  
  Value value( node->attr( "value" ).value(), node->attr( "measure" ).as< string >() );
  string name( node->attr( "name" ).as< string >() );
  _values[ name ].push_back( value );
  
  if( _values[ name ].size() != _iterations.size() )
    LOG( ERROR ) << "Value with name " << name << " missed.";
}

void Phase::subphaseFromXml( xml::Node& node )
{
  if( !node.attr( "name" ).isSet() )
    LOG( ERROR ) << "Name attribute for child phase must be set.";
    
  PhasePtr childPhase( Phase::fromXml( node ) );
  string name( node.attr( "name" ).as< string >() );
  _phases[ name ].push_back( childPhase );
    
  if( _phases[ name ].size() != _iterations.size() )
    LOG( ERROR ) << "Phase with name " << name << " missed.";
}

void Phase::iterationFromXml( xml::Node& node )
{
  if( !node.attr( "name" ).isSet() )
    LOG( ERROR ) << "Name attribute for iteration not set.";
  
  _iterations.push_back( node.attr( "name" ).value() );
  
  BOOST_FOREACH( xml::NodePtr child, node.childs( "value" ) )
    setValueFromXml( child );
  BOOST_FOREACH( xml::NodePtr child, node.childs( "phase" ) )
    subphaseFromXml( *child );
  BOOST_FOREACH( xml::NodePtr child, node.childs( "loop" ) )
    subphaseFromXml( *child );
    
  if( _values[ "time" ].size() < _iterations.size() )
    LOG( ERROR ) << "Time value must be set for each iteration.";
}

PhasePtr Phase::fromXml( xml::Node& node )
{
  if( node.name() == "phase" )
  {
    PhasePtr ret( new Phase() );
    ret->iterationFromXml( node );
    ret->_iterations[ 0 ] = "";
    return ret;
  }
  else if( node.name() == "loop" )
  {
    PhasePtr ret( new Phase() );
    BOOST_FOREACH( xml::NodePtr iter, node.childs( "iteration" ) )
      ret->iterationFromXml( *iter );
      
    if( node.childs( "iteration" ).count() != node.childs().count() )
      LOG( ERROR ) << "Loop xml nodes can have only iteration childs.";
    
    return ret;
  }
  
  LOG( ERROR ) << "Cannot create phase from tag " << node.name() << '.';
  return PhasePtr();
}

void Phase::toTable( Table* table )
{
  assert( table != NULL );
  
  iterationsToTable( table );
  
  std::pair< string, vector< Value > > value;
  BOOST_FOREACH( value, _values )
  {
    Table::ColumnProxy column( table->newColumn() );
    
    column.name() = value.first;
    
    BOOST_FOREACH( Value val, value.second )
      column.pushBack( val );
  }
  
  std::pair< string, vector< PhasePtr > > phaseVec;
  BOOST_FOREACH( phaseVec, _phases )
  {
    Table::ColumnProxy column( table->newColumn() );
    
    column.name() = phaseVec.first;
    BOOST_FOREACH( PhasePtr phase, phaseVec.second )
    {
      if( phase->_iterations.size() == 1 )
	column.pushBack( phase->_values[ "time" ][ 0 ] );
    }
  }
}

bool Phase::isLoop()
{
  if( _iterations.size() > 1 )
    return true;
  if( _iterations.size() == 0 )
    return false;
  
  return !( _iterations[ 0 ] == "" );
}

void Phase::iterationsToTable( Table* table )
{
  if( _iterations.size() < 1 )
    return;
  
  if( _iterations.size() == 1 && _iterations[ 0 ] == "" )
    return;
    
  Table::ColumnProxy column( table->newColumn() );
  
  BOOST_FOREACH( xml::Attribute iteration, _iterations )
    column.pushBack( iteration.value() );
}
