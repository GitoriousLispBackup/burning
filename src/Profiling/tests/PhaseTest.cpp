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

#include <sstream>
#include <gtest/gtest.h>
#include <boost/tr1/memory.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/std/map.hpp>
#include <boost/foreach.hpp>
#include <Profiling/Table.hpp>
#include <Profiling/Phase.hpp>

using std::string;
using std::vector;
using std::tr1::shared_ptr;
using namespace boost::assign;
using namespace burning;
using namespace burning::profiling;

class PhaseTest : public testing::Test
{
public:
  void SetUp();
  
  Phase phase;
  Table table;
  std::stringstream stream;
};

void PhaseTest::SetUp()
{
}

TEST_F( PhaseTest, Initialization )
{
  EXPECT_EQ( phase.values().size(), 0 );
  EXPECT_EQ( phase.phases().size(), 0 );
}

const vector< Value >& findValue( const Phase::ValueMap& map, const string& name )
{
  Phase::ValueMap::const_iterator iterator( map.find( name ));
  return iterator->second;
}

TEST_F( PhaseTest, Time )
{
  phase.beginIteration( "" );
  phase.endIteration();
  
  ASSERT_EQ( phase.values().size(), 1 );
  EXPECT_EQ( findValue( phase.values(), "time" )[ 0 ].measure(), "ms" );
}

TEST_F( PhaseTest, AddValueWithoutIteration )
{
  EXPECT_EXIT( phase.addValue( "test", 42 ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( PhaseTest, Subphase )
{
  PhasePtr subphase( new Phase() );
  
  phase.beginIteration( "" );
  phase.addPhase( "subphase", subphase );
  
  subphase->beginIteration( "" );
  subphase->endIteration();
  phase.endIteration();
  
  ASSERT_EQ( phase.phases().size(), 1 );
  ASSERT_EQ( phase.phases().count( "subphase" ), 1 );
}

TEST_F( PhaseTest, MissedBegin )
{
  EXPECT_EXIT( phase.endIteration(), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( PhaseTest, UnexpectedBegin )
{
  phase.beginIteration( "" );
  EXPECT_EXIT( phase.beginIteration( "" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( PhaseTest, IterationTest )
{
  phase.beginIteration( 1 );
  phase.endIteration();
  
  ASSERT_EQ( phase.iterations().size(), 1 );
  EXPECT_EQ( phase.iterations()[ 0 ], 1 );
  ASSERT_EQ( phase.values().size(), 1 );
}

TEST_F( PhaseTest, TwoIterationsTest )
{
  for( int i=0; i<2; i++ )
  {
    phase.beginIteration( 2*i + 1 );
    phase.addValue( "test", 42*i );
    phase.endIteration();
  }
  
  ASSERT_EQ( phase.iterations().size(), 2 );
  EXPECT_EQ( phase.iterations()[ 0 ], 1 );
  EXPECT_EQ( phase.iterations()[ 1 ], 3 );
  ASSERT_EQ( phase.values().size(), 2 );
  ASSERT_EQ( findValue( phase.values(), "test" ).size(), 2 );
  EXPECT_EQ( findValue( phase.values(), "test" )[ 0 ].value(), 0 );
  EXPECT_EQ( findValue( phase.values(), "test" )[ 1 ].value(), 42 );
}

TEST_F( PhaseTest, TooLittleValues )
{
  phase.beginIteration( 1 );
  phase.endIteration();
  
  phase.beginIteration( 2 );
  EXPECT_EXIT( phase.addValue( "test", 42 ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

const vector< PhasePtr >& findPhase( const Phase::PhaseMap& map, const string& name )
{
  Phase::PhaseMap::const_iterator iterator( map.find( name ) );
  return iterator->second;
}

TEST_F( PhaseTest, SubphasesIteration )
{
  for( int i=0; i<2; i++ )
  {
    phase.beginIteration( 2*i+1 );
    
    PhasePtr subphase( new Phase() );
    subphase->beginIteration( "" );
    subphase->endIteration();
    
    phase.addPhase( "test", subphase );
    
    phase.endIteration();
  }
  
  ASSERT_EQ( phase.phases().count( "test" ), 1 );
  ASSERT_EQ( findPhase( phase.phases(), "test" ).size(), 2 );
  
  const PhasePtr& subiteration1( findPhase( phase.phases(), "test" )[ 0 ] );
  EXPECT_EQ( findValue( subiteration1->values(), "time" ).size(), 1 );
  const PhasePtr& subiteration2( findPhase( phase.phases(), "test" )[ 1 ] );
  EXPECT_EQ( findValue( subiteration2->values(), "time" ).size(), 1 );
}

TEST_F( PhaseTest, TooLittleSubphasesIterations )
{
  phase.beginIteration( 1 );
  phase.endIteration();
  
  phase.beginIteration( 2 );
  EXPECT_EXIT( phase.addPhase( "test", PhasePtr() ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( PhaseTest, TooMuchSubphases )
{
  phase.beginIteration( 1 );
  phase.addPhase( "test", PhasePtr() );
  EXPECT_EXIT( phase.addPhase( "test", PhasePtr() ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( PhaseTest, NestedIterations )
{
  phase.beginIteration( 1 );
  EXPECT_EXIT( phase.beginIteration( 2 ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( PhaseTest, SimpleXml )
{
  xml::NodePtr xml( phase.toXml() );
  
  EXPECT_EQ( xml->name(), "phase" );
  EXPECT_EQ( xml->childs().count(), 0 );
}

void checkTime( xml::NodePtr node )
{
  int timeNodes( 0 );
  BOOST_FOREACH( xml::NodePtr child, node->childs( "value" ) )
  {
    if( child->attr( "name" ) == "time" )
    {
      timeNodes++;
    }
    
    EXPECT_EQ( child->attr( "measure" ), "ms" );
  }
  EXPECT_EQ( timeNodes, 1 );
}

TEST_F( PhaseTest, XmlWithEmptyIteration )
{
  phase.beginIteration( "" );
  phase.endIteration();
  
  xml::NodePtr xml( phase.toXml() );
  
  EXPECT_EQ( xml->name(), "phase" );
  EXPECT_EQ( xml->childs().count(), 1 );
  checkTime( xml );
}

TEST_F( PhaseTest, XmlWithIteration )
{
  phase.beginIteration( "test" );
  phase.endIteration();
  
  xml::NodePtr xml( phase.toXml() );
  
  EXPECT_EQ( xml->name(), "loop" );
  EXPECT_EQ( xml->childs().count(), 1 );
  ASSERT_EQ( xml->childs( "iteration" ).count(), 1 );
  
  xml::NodePtr iter( *xml->childs( "iteration" ).begin() );
  EXPECT_EQ( iter->name(), "iteration" );
  checkTime( iter );
  
  EXPECT_EQ( iter->attr( "name" ), "test" );
}

TEST_F( PhaseTest, XmlWithIterations )
{
  phase.beginIteration( "test1" );
  phase.endIteration();
  
  phase.beginIteration( "test2" );
  phase.endIteration();
  
  xml::NodePtr xml( phase.toXml() );
  
  EXPECT_EQ( xml->name(), "loop" );
  EXPECT_EQ( xml->childs().count(), 2 );
  ASSERT_EQ( xml->childs( "iteration" ).count(), 2 );
  
  xml::NodeSet::iterator childIterator( xml->childs( "iteration" ).begin() );
  
  xml::NodePtr child1( *childIterator );
  EXPECT_EQ( child1->name(), "iteration" );
  checkTime( child1 );
  EXPECT_EQ( child1->attr( "name" ), "test1" );
  
  xml::NodePtr child2( *(++childIterator) );
  EXPECT_EQ( child2->name(), "iteration" );
  checkTime( child2 );
  EXPECT_EQ( child2->attr( "name" ), "test2" );
}

TEST_F( PhaseTest, XmlWithPhases )
{
  phase.beginIteration( "test" );
  
  PhasePtr subphase( new Phase() );
  phase.addPhase( "some phase", subphase );
  
  phase.endIteration();
  
  xml::NodePtr xml( phase.toXml() );
  EXPECT_EQ( xml->name(), "loop" );
  EXPECT_EQ( xml->childs().count(), 1 );
  EXPECT_EQ( xml->childs( "iteration" ).count(), 1 );
  
  xml::NodePtr iter( *xml->childs( "iteration" ).begin() );
  checkTime( iter );
  
  EXPECT_EQ( iter->childs().count(), 2 );
  EXPECT_EQ( iter->childs( "phase" ).count(), 1 );
  
  xml::NodePtr subXml( *iter->childs( "phase" ).begin() );
  EXPECT_EQ( subXml->attr( "name" ), "some phase" );
}

TEST_F( PhaseTest, XmlWithMissedPhases )
{
  phase.beginIteration( "test1" );
  
  PhasePtr subphase( new Phase() );
  phase.addPhase( "some phase", subphase );
  
  phase.endIteration();
  
  phase.beginIteration( "failed" );
  phase.endIteration();
  
  EXPECT_EXIT( phase.toXml(), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( PhaseTest, XmlWithValue )
{
  phase.beginIteration( "" );
  
  phase.addValue( "test" , "some string" );
  
  phase.endIteration();
  
  xml::NodePtr xml( phase.toXml() );
  
  EXPECT_EQ( xml->name(), "phase" );
  EXPECT_EQ( xml->childs().count(), 2 );
  ASSERT_EQ( xml->childs( "value" ).count(), 2 );
  
  int testNodes( 0 );
  BOOST_FOREACH( xml::NodePtr child, xml->childs( "value" ) )
  {
    if( child->attr( "name" ) == "test" )
    {
      testNodes++;
      EXPECT_EQ( child->attr( "value" ), "some string" );
      EXPECT_FALSE( child->attr( "measure" ).isSet() );
    }
  }
  EXPECT_EQ( testNodes, 1 );
}

TEST_F( PhaseTest, XmlWithDecimalValue )
{
  phase.beginIteration( "" );
  
  phase.addValue( "test" , 42 );
  
  phase.endIteration();
  
  xml::NodePtr xml( phase.toXml() );
  
  EXPECT_EQ( xml->name(), "phase" );
  EXPECT_EQ( xml->childs().count(), 2 );
  ASSERT_EQ( xml->childs( "value" ).count(), 2 );
  
  int testNodes( 0 );
  BOOST_FOREACH( xml::NodePtr child, xml->childs( "value" ) )
  {
    if( child->attr( "name" ) == "test" )
    {
      testNodes++;
      EXPECT_EQ( child->attr( "value" ), 42 );
      EXPECT_FALSE( child->attr( "measure" ).isSet() );
    }
  }
  EXPECT_EQ( testNodes, 1 );
}

TEST_F( PhaseTest, XmlWithMissedValues )
{
  phase.beginIteration( "one" );
  phase.addValue( "test" , 42 );
  phase.endIteration();
  
  phase.beginIteration( "ooops" );
  phase.endIteration();
  
  EXPECT_EXIT( phase.toXml(), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( PhaseTest, XmlWithMeasuredValue )
{
  phase.beginIteration( "" );
  
  phase.addValue( "test" , Value( "some string", "letters" ) );
  
  phase.endIteration();
  
  xml::NodePtr xml( phase.toXml() );
  
  EXPECT_EQ( xml->name(), "phase" );
  EXPECT_EQ( xml->childs().count(), 2 );
  ASSERT_EQ( xml->childs( "value" ).count(), 2 );
  
  int testNodes( 0 );
  BOOST_FOREACH( xml::NodePtr child, xml->childs( "value" ) )
  {
    if( child->attr( "name" ) == "test" )
    {
      testNodes++;
      EXPECT_EQ( child->attr( "value" ), "some string" );
      EXPECT_EQ( child->attr( "measure" ), "letters" );
    }
  }
  EXPECT_EQ( testNodes, 1 );
}

TEST_F( PhaseTest, XmlWithIntergerIteration )
{
  phase.beginIteration( 1 );
  phase.endIteration();
  
  xml::NodePtr xml( phase.toXml() );
  
  ASSERT_EQ( xml->childs( "iteration" ).count(), 1 );
  
  xml::NodePtr child( *xml->childs( "iteration" ).begin() );
  EXPECT_EQ( child->attr( "name" ), 1 );
}

TEST_F( PhaseTest, FromXml )
{
  xml::Node xml( "loop" );
  PhasePtr phase( Phase::fromXml( xml ) );
  
  ASSERT_FALSE( phase == NULL );
  EXPECT_EQ( phase->iterations().size(), 0 );
  EXPECT_EQ( phase->values().size(), 0 );
  EXPECT_EQ( phase->phases().size(), 0 );
}

xml::NodePtr createTime()
{
  xml::NodePtr ret( xml::Node::create( "value" ) );
  
  ret->attr( "name" ) = "time";
  ret->attr( "value" ) = 10;
  ret->attr( "measure" ) = "milliseconds";
  
  return ret;
}

TEST_F( PhaseTest, FromXmlPhase )
{
  xml::Node xml( "phase" );
  xml.attr( "name" ) = "simple phase";
  xml.childs() += createTime();
  
  PhasePtr phase( Phase::fromXml( xml ) );
  
  ASSERT_FALSE( phase == NULL );
  ASSERT_EQ( phase->iterations().size(), 1 );
  EXPECT_EQ( phase->iterations()[ 0 ], "" );
  EXPECT_EQ( phase->values().size(), 1 );
  EXPECT_EQ( phase->values().count( "time" ), 1 );
  EXPECT_EQ( phase->phases().size(), 0 );
}

TEST_F( PhaseTest, FromXmlLoop )
{
  xml::Node xml( "loop" );
  
  xml::NodePtr iteration( xml::Node::create( "iteration" ) );
  iteration->attr( "name" ) = "iter";
  iteration->childs() += createTime();
  
  xml.childs() += iteration;
  
  PhasePtr phase( Phase::fromXml( xml ) );
  
  ASSERT_FALSE( phase == NULL );
  ASSERT_EQ( phase->iterations().size(), 1 );
  EXPECT_EQ( phase->iterations()[ 0 ], "iter" );
  EXPECT_EQ( phase->values().size(), 1 );
  ASSERT_EQ( phase->values().count( "time" ), 1 );
  EXPECT_EQ( phase->phases().size(), 0 );
}

TEST_F( PhaseTest, SubphaseFromXml )
{
  xml::Node xml( "phase" );
  xml.attr( "name" ) = "parent phase";
  xml.childs() += createTime();
  
  xml::NodePtr subphase( xml::Node::create( "phase" ) );
  subphase->attr( "name" ) = "some phase";
  subphase->childs() += createTime();
  
  xml.childs() += subphase;
  
  PhasePtr phase( Phase::fromXml( xml ) );
  
  ASSERT_FALSE( phase == NULL );
  EXPECT_EQ( phase->phases().size(), 1 );
  EXPECT_EQ( phase->phases().count( "some phase" ), 1 );
}

TEST_F( PhaseTest, SimpleTable )
{
  phase.toTable( &table );
  
  EXPECT_EQ( table.columns(), 0 );
  EXPECT_EQ( table.rows(), 0 );
}

TEST_F( PhaseTest, ValueTable )
{
  
  phase.beginIteration( "" );
  phase.addValue( "test", Value( 1, "measured" ) );
  
  phase.toTable( &table );
  ASSERT_EQ( table.columns(), 1 );
  ASSERT_EQ( table.rows(), 1 );
  EXPECT_EQ( table.column( 0 ).name(), "test" );
  EXPECT_EQ( table[ 0 ][ 0 ].measure(), "measured" );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), 1 );
}

TEST_F( PhaseTest, PhasesTable )
{
  phase.beginIteration( "" );
  
  PhasePtr subphase( new Phase() );
  phase.addPhase( "subphase", subphase );
  
  subphase->beginIteration( "" );
  subphase->endIteration();
  
  phase.endIteration();
  
  phase.toTable( &table );
  
  ASSERT_EQ( table.rows(), 1 );
  ASSERT_EQ( table.columns(), 2 );
  EXPECT_EQ( table.column( 0 ).name(), "time" );
  EXPECT_EQ( table.column( 1 ).name(), "subphase" );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), 0 );
  EXPECT_EQ( table[ 0 ][ 0 ].measure(), "ms" );
  EXPECT_EQ( table[ 0 ][ 1 ].value(), 0 );
  EXPECT_EQ( table[ 0 ][ 1 ].measure(), "ms" );
}

TEST_F( PhaseTest, IterationsTable )
{
  phase.beginIteration( "first" );
  phase.addValue( "value", 1 );
  phase.endIteration();
  
  phase.beginIteration( "second" );
  phase.addValue( "value", 2 );
  phase.endIteration();
  
  phase.toTable( &table );
  
  ASSERT_EQ( table.rows(), 2 );
  ASSERT_EQ( table.columns(), 3 );
  EXPECT_EQ( table.column( 1 ).name(), "time" );
  EXPECT_EQ( table.column( 2 ).name(), "value" );
  EXPECT_EQ( table[ 0 ][ 2 ].value(), 1 );
  EXPECT_EQ( table[ 1 ][ 2 ].value(), 2 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "first" );
  EXPECT_EQ( table[ 1 ][ 0 ].value(), "second" );
}
