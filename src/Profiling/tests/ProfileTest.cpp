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

#include <gtest/gtest.h>
#include <Profiling/Profile.hpp>

using std::string;
using namespace burning;
using namespace burning::profiling;

class ProfileTest : public testing::Test
{
public:
  void SetUp();
  
  Profile profile;
  const profiling::Phase* root;
};

void ProfileTest::SetUp()
{
  root = &profile.rootPhase();
}

TEST_F( ProfileTest, Initialization )
{
  EXPECT_EQ( root->values().size(), 0 );
  EXPECT_EQ( root->phases().size(), 0 );
  EXPECT_EQ( root->iterations().size(), 1 );
}

void checkTime( const Phase::PhaseMap& map, const string& name, size_t iterations = 1 )
{
  ASSERT_EQ( map.count( name ), 1 );
  Phase::PhaseMap::const_iterator it( map.find( name ) );
  
  ASSERT_EQ( it->second.size(), 1 );
  ASSERT_EQ( it->second[ 0 ]->values().count( "time" ), 1 );
  
  Phase::ValueMap::const_iterator time( it->second[ 0 ]->values().find( "time" ) );
  ASSERT_EQ( time->second.size(), iterations );
}

TEST_F( ProfileTest, SimplePhaseTest )
{
  profile.beginPhase( "test" );
  profile.endPhase();
  
  checkTime( root->phases(), "test" );
}

TEST_F( ProfileTest, AddValueTest )
{
  profile.addValue( "test", 42 );
  
  ASSERT_EQ( root->values().size(), 1 );
}

TEST_F( ProfileTest, MissedBeginPhase )
{
  EXPECT_EXIT( profile.endPhase(), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}
  
TEST_F( ProfileTest, TooMushEndPhases )
{
  profile.beginPhase( "test" );
  profile.endPhase();
  
  EXPECT_EXIT( profile.endPhase(), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ProfileTest, NestedPhases )
{
  profile.beginPhase( "test1" );
  profile.beginPhase( "test2" );
  profile.endPhase();
  profile.endPhase();
  
  ASSERT_EQ( root->phases().size(), 1 );
  ASSERT_EQ( root->phases().count( "test1" ), 1 );
  ASSERT_EQ( root->values().size(), 0 );
  
  checkTime( root->phases(), "test1" );
  Phase::PhaseMap::const_iterator it = root->phases().find( "test1" );
  ASSERT_EQ( it->second.size(), 1 );
  const Phase& nested( *( it->second[ 0 ] ) );
  
  ASSERT_EQ( nested.phases().size(), 1 );
  checkTime( nested.phases(), "test2" );
}

TEST_F( ProfileTest, Loop )
{
  profile.beginLoop( "loop" );
  profile.beginIteration( 1 );
  profile.endIteration();
  profile.beginIteration( 2 );
  profile.endIteration();
  profile.endLoop();
  
  ASSERT_EQ( root->phases().size(), 1 );
  ASSERT_EQ( root->phases().count( "loop" ), 1 );
  
  checkTime( root->phases(), "loop", 2 );
}

TEST_F( ProfileTest, LoopWithoutIterations )
{
  profile.beginLoop( "loop" );
  EXPECT_EXIT( profile.addValue( "fail", "fail" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ProfileTest, IterationWithoutLoop )
{
  EXPECT_EXIT( profile.beginIteration( "fail" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ProfileTest, UnfinishedLoop )
{
  profile.beginLoop( "loop" );
  profile.beginIteration( 123 );
  EXPECT_EXIT( profile.endLoop(), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ProfileTest, SimpleXml )
{
  profile.beginPhase( "simplePhase" );
  profile.endPhase();
  
  xml::NodePtr xml( profile.toXml() );
  
  EXPECT_EQ( xml->name(), "profile" );
  EXPECT_EQ( xml->childs().count(), 2 );
  ASSERT_EQ( xml->childs( "phase" ).count(), 1 );
}

TEST_F( ProfileTest, UnfinishedToXml )
{
  profile.beginPhase( "simplePhase" );
  profile.beginPhase( "phase2" );
  profile.endPhase();
  
  EXPECT_EXIT( profile.toXml(), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ProfileTest, FromXml )
{
  xml::Node xml( "profile" );
  ProfilePtr profile( Profile::fromXml( xml ) );
  
  ASSERT_FALSE( profile == NULL );
  EXPECT_EQ( profile->rootPhase().iterations().size(), 1 );
}

TEST_F( ProfileTest, ComplexFromXml )
{
  xml::Node xml( "profile" );
  
  xml::NodePtr phase( xml::Node::create( "phase" ) );
  phase->attr( "name" ) = "subphase";
  
  xml.childs() += phase;
  
  ProfilePtr profile( Profile::fromXml( xml ) );
  
  ASSERT_FALSE( profile == NULL );
  EXPECT_EQ( profile->rootPhase().iterations().size(), 1 );
  EXPECT_EQ( profile->rootPhase().phases().size(), 1 );
}
