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
#include <set>
#include <list>
#include <CommandLine/CommandLine.hpp>

using std::vector;
using std::set;
using std::list;
using std::string;
using std::stringstream;
using namespace burning;
using namespace burning::commandLine;

class CommandLineTest : public testing::Test
{
public:
  void SetUp();
  
  CommandLine commandLine;
};

void CommandLineTest::SetUp()
{
}

TEST_F( CommandLineTest, SimpleParse )
{
  int argc( 1 );
  const char* argv[] = { "CommandLineTest" };
  
  commandLine.parse( argc, argv );
}

TEST_F( CommandLineTest, SimpleFlag )
{
  int argc( 2 );
  const char* argv[] = { "Test", "--flag" };
  
  commandLine.arguments() += Flag( "flag" );
  commandLine.arguments() += Flag( "other flag" );
			     
  commandLine.parse( argc, argv );
  
  EXPECT_TRUE( commandLine.argument( "flag" ).isSet() );
  EXPECT_FALSE( commandLine.argument( "other flag" ).isSet() );
}

TEST_F( CommandLineTest, WrongArgument )
{
  int argc( 2 );
  const char* argv[] = { "Test" , "a value" };
  
  EXPECT_EXIT( commandLine.parse( argc, argv ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( CommandLineTest, IgnoreFirst )
{
  int argc( 1 );
  const char* argv[] = { "--flag" };
  
  commandLine.arguments() += Flag( "flag" );
  commandLine.parse( argc, argv );
  
  EXPECT_FALSE( commandLine.argument( "flag" ).isSet() );
}

TEST_F( CommandLineTest, NonExistingFlag )
{
  const char* argv[] = { "test", "--flag" };
  EXPECT_EXIT( commandLine.parse( 2, argv ), testing::ExitedWithCode( EXIT_FAILURE ), "" ); 
}

TEST_F( CommandLineTest, ShortName )
{
  const char* argv[] = { "test" , "-f" };
  commandLine.arguments() += Flag( "flag", 'f' );
  commandLine.parse( 2, argv );
  
  EXPECT_TRUE( commandLine.argument( "flag" ).isSet() );
}

TEST_F( CommandLineTest, NonExistingShortName )
{
  const char* argv[] = { "test", "-f" };
  EXPECT_EXIT( commandLine.parse( 2, argv ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( CommandLineTest, MultipleShortNames )
{
  commandLine.arguments() += Flag( "flagA", 'a' ), Flag( "flagB", 'b' );
  
  const char* argv[] = { "test" , "-ab" };
  commandLine.parse( 2, argv );
  
  EXPECT_TRUE( commandLine.argument( "flagA" ).isSet() );
  EXPECT_TRUE( commandLine.argument( "flagB" ).isSet() );
}

TEST_F( CommandLineTest, HelpFlag )
{
  const char* argv[] = { "test", "--help" };
  commandLine.parse( 2, argv );
  
  EXPECT_TRUE( commandLine.argument( "help" ).isSet() );
}

TEST_F( CommandLineTest, SimpleKey )
{
  const char* argv[] = { "test", "--key", "10" };
  
  commandLine.arguments() += Key< int >( "key" );
  commandLine.parse( 3, argv );
  
  EXPECT_TRUE( commandLine.argument( "key" ).isSet() );
}

TEST_F( CommandLineTest, BadKeyValue )
{
  const char* argv[] = { "test", "--key", "0.1" };
  
  commandLine.arguments() += Key< int >( "key" );
  EXPECT_THROW( commandLine.parse( 3, argv ), boost::bad_lexical_cast );
}

TEST_F( CommandLineTest, MissedKeyValue )
{
  const char* argv[] = { "test", "--key" };
  
  commandLine.arguments() += Key< int >( "key" );
  EXPECT_EXIT( commandLine.parse( 2, argv ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( CommandLineTest, KeyAs )
{
  const char* argv[] = { "test", "--key", "42" };
  
  commandLine.arguments() += Key< int >( "key" );
  commandLine.parse( 3, argv );
  
  EXPECT_EQ( commandLine.argument( "key" ).as< int >(), 42 );
}

TEST_F( CommandLineTest, VoidAs )
{
  commandLine.arguments() += Flag( "flag" );
  EXPECT_EXIT( commandLine[ "flag" ].as< int >(), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( CommandLineTest, StringKey )
{
  const char* argv[] = { "test", "--key", "some key value" };
  
  commandLine.arguments() += Key< string >( "key" );
  commandLine.parse( 3, argv );
  
  EXPECT_EQ( commandLine[ "key" ].as< string >(), "some key value" );
}

TEST_F( CommandLineTest, VectorKey )
{
  const char* argv[] = { "test", "--key", "value1", "value2" };
  
  commandLine.arguments() += Key< vector< string > >( "key" );
  commandLine.parse( 4, argv );
  
  vector< string > key( commandLine[ "key" ].as< vector< string > >() );
  ASSERT_EQ( key.size(), 2 );
  EXPECT_EQ( key[ 0 ], "value1" );
  EXPECT_EQ( key[ 1 ], "value2" );
}

TEST_F( CommandLineTest, FlagAfterVector )
{
  const char* argv[] = { "test", "--key", "value", "-f" };
  
  commandLine.arguments() += Key< vector< string > >( "key" ), Flag( "flag", 'f' );
  commandLine.parse( 4, argv );
  
  vector< string > key( commandLine[ "key" ].as< vector< string > >() );
  
  EXPECT_EQ( key.size(), 1 );
  EXPECT_EQ( key[ 0 ], "value" );
  EXPECT_TRUE( commandLine[ "flag" ].isSet() );
}

TEST_F( CommandLineTest, ListKey )
{
  const char* argv[] = { "test", "--key", "23", "34" };
  
  commandLine.arguments() += Key< list< int > >( "key" );
  commandLine.parse( 4, argv );
  
  list< int > key( commandLine[ "key" ].as< list< int > >() );
  ASSERT_EQ( key.size(), 2 );
  EXPECT_EQ( key.front(), 23 );
  EXPECT_EQ( key.back(), 34 );
}

TEST_F( CommandLineTest, SetKey )
{
  const char* argv[] = { "test", "--key", "23", "34" };
  
  commandLine.arguments() += Key< set< int > >( "key" );
  commandLine.parse( 4, argv );
  
  set< int > key( commandLine[ "key" ].as< set< int > >() );
  ASSERT_EQ( key.size(), 2 );
  EXPECT_EQ( key.count( 23 ), 1 );
  EXPECT_EQ( key.count( 34 ), 1 );
}

TEST_F( CommandLineTest, ParseActionArgument )
{
  const char* argv[] = { "test", "act" };
  
  commandLine.arguments() += Action( "act" );
  commandLine.parse( 2, argv );
  
  EXPECT_TRUE( commandLine.action( "act" ).isSet() );
}

TEST_F( CommandLineTest, ParseActionsArgument )
{
  const char* argv[] = { "test", "act", "--arg" };
  Action action( "act" );
  action.arguments() += Flag( "arg" );
  
  commandLine.arguments() += action;
  commandLine.parse( 3, argv );
  
  ASSERT_TRUE( commandLine.action( "act" ).isSet() );
  EXPECT_TRUE( commandLine.action( "act" )[ "arg" ].isSet() );
}

TEST_F( CommandLineTest, ArgumentForNonSetAction )
{
  const char* argv[] = { "test" , "--arg" };
  
  Action action( "act" );
  action.arguments() += Flag( "arg" );
  commandLine.arguments() += action;
  EXPECT_EXIT( commandLine.parse( 2, argv ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( CommandLineTest, SubactionsSameArgument )
{
  const char* argv[] = { "test", "act" , "--arg" };
  
  Action action( "act" );
  action.arguments() += Flag( "arg" );
  
  commandLine.arguments() += Flag( "arg" );
  commandLine.arguments() += action;
  commandLine.parse( 3, argv );
  
  EXPECT_FALSE( commandLine[ "arg" ].isSet() );
  ASSERT_TRUE( commandLine.action( "act" ).isSet() );
  EXPECT_TRUE( commandLine.action( "act" )[ "arg" ].isSet() );
}

TEST_F( CommandLineTest, PositionalParse )
{
  const char* argv[] = { "test", "bla-bla-bla" };
  
  commandLine.positionals() += Key< string >( "key" );
  commandLine.parse( 2, argv );
  
  EXPECT_TRUE( commandLine.positional( "key" ).isSet() );
  EXPECT_EQ( commandLine.positional( "key" ).as< string >(), "bla-bla-bla" );
}

TEST_F( CommandLineTest, PositionalSubactionParse )
{
  const char* argv[] = { "test", "act", "42", "value" };
  
  Action action( "act" );
  action.positionals() += Key< int >( "key" );
  action.positionals() += Key< string >( "key2" );
  
  commandLine.arguments() += action;
  commandLine.parse( 4, argv );
  
  EXPECT_TRUE( commandLine.action( "act" ).isSet() );
  Action& act( commandLine.action( "act" ) );
  EXPECT_TRUE( act.positional( "key" ).isSet() );
  EXPECT_EQ( act.positional( "key" ).as< int >(), 42 ); 
  EXPECT_TRUE( act.positional( "key2" ).isSet() );
  EXPECT_EQ( act.positional( "key2" ).as< string >(), "value" ); 
}
