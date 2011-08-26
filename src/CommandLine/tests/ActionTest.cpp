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
#include <CommandLine/Action.hpp>

using std::vector;
using std::set;
using std::list;
using std::string;
using std::stringstream;
using namespace burning;
using namespace burning::commandLine;

class ActionTest : public testing::Test
{
public:
  void SetUp();
  
  Action action;
};

void ActionTest::SetUp()
{
}

TEST_F( ActionTest, SimpleFlag )
{
  action.arguments() += Flag( "flag" );
  action.arguments() += Flag( "other flag" );
			     
  EXPECT_FALSE( action.argument( "flag" ).isSet() );
  EXPECT_FALSE( action.argument( "other flag" ).isSet() );
}

TEST_F( ActionTest, IndexAccess )
{
  action.arguments() += Flag( "flag" );
  
  EXPECT_FALSE( action[ "flag" ].isSet() );
}

TEST_F( ActionTest, NonExistingArgument )
{
  EXPECT_EXIT( action.argument( "flag" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, ArgumentDescription )
{
  action.arguments() += Flag( "flag", "test flag" );
  EXPECT_EQ( action.argument( "flag" ).description(), "test flag" ); 
}

TEST_F( ActionTest, AddFlagTwice )
{
  action.arguments() += Flag( "flag" );
  EXPECT_EXIT( action.arguments() += Flag( "flag" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, HelpMessage )
{
  string expected( "Usage:\n"\
                   "   [ARGS]\n\n"\
                   "  Where ARGS are:\n"\
		   "\t--help              \tProducts this help message\n\n" 
		 );
  
  stringstream stream;
  action.printHelp( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( ActionTest, FlagHelp )
{
  string expected( "Usage:\n"\
                   "   [ARGS]\n\n"\
                   "  Where ARGS are:\n"\
                   "\t--flag              \tSome flag\n"
                   "\t--help              \tProducts this help message\n\n" );
  
  stringstream stream;
  
  action.arguments() += Flag( "flag", "Some flag" );
  action.printHelp( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( ActionTest, CommaSeparatedAgrumentsInsert )
{
  action.arguments() += Flag( "flag" ), Flag( "flag2" );
  
  EXPECT_FALSE( action.argument( "flag" ).isSet() ); 
  EXPECT_FALSE( action.argument( "flag2" ).isSet() );
}

TEST_F( ActionTest, EqualShortNames )
{
  action.arguments() += Flag( "flag" , 'f' );
  EXPECT_EXIT( action.arguments() += Flag( "other flag" , 'f' ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, SetHelpFlag )
{
  action.helpFlag() = Flag( "help", 'h', "My help message" );
  EXPECT_FALSE( action.helpFlag().isSet() );
}

TEST_F( ActionTest, DisableHelp )
{
  action.disableHelp();
  EXPECT_EXIT( action.argument( "help" ).isSet() , testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, EnableHelp )
{
  action.disableHelp();
  action.enableHelp();
  
  EXPECT_FALSE( action.argument( "help" ).isSet() );
}

TEST_F( ActionTest, PrintShortNamesInHelp )
{
  string expected( "Usage:\n"\
                   "   [ARGS]\n\n"\
                   "  Where ARGS are:\n"\
                   "\t--flag,-f           \tsome flag\n"\
                   "\t--help              \tProducts this help message\n\n"
		 );
  
  action.arguments() += Flag( "flag", 'f', "some flag" );
  
  stringstream stream;
  action.printHelp( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( ActionTest, PrintKeyHelp )
{
  string expected( "Usage:\n"\
                   "   [ARGS]\n\n"\
                   "  Where ARGS are:\n"\
                   "\t--help              \tProducts this help message\n"
                   "\t--key PARAM         \tsome key\n\n"\
		 );
  
  action.arguments() += Key< int >( "key", "some key" );
  
  stringstream stream;
  action.printHelp( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( ActionTest, PrintVectorKeyHelp )
{
  string expected( "Usage:\n"\
                   "   [ARGS]\n\n"\
                   "  Where ARGS are:\n"\
                   "\t--help              \tProducts this help message\n"
                   "\t--key [PARAM1 ...]  \tsome key\n\n"\
		 );
  
  action.arguments() += Key< vector< int > >( "key", "some key" );
  
  stringstream stream;
  action.printHelp( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( ActionTest, NameInHelp )
{
  string expected( "Usage:\n"\
                   "  action [ARGS]\n\n"\
                   "  Where ARGS are:\n"\
                   "\t--help              \tProducts this help message\n\n"
		 );
  
  Action action( "action" );
  
  stringstream stream;
  action.printHelp( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( ActionTest, Name )
{
  Action action( "action" );
  EXPECT_EQ( action.name(), "action" );
}

TEST_F( ActionTest, SetName )
{
  action.name() = "some action";
  EXPECT_EQ( action.name(), "some action" );
}

TEST_F( ActionTest, ActionInsert )
{
  action.arguments() += Action( "new action" );
  
  EXPECT_FALSE( action.action( "new action" ).isSet() );
}

TEST_F( ActionTest, InsertSameAction )
{
  action.arguments() += Action( "action" );
  EXPECT_EXIT( action.arguments() += Action( "action" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, NotExistingActionCheck )
{
  EXPECT_EXIT( action.action( "action" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, CommaSeparatedActionInsert )
{
  action.arguments() += Action( "action1" ), Action( "action2" );
  
  EXPECT_FALSE( action.action( "action1" ).isSet() );
  EXPECT_FALSE( action.action( "action2" ).isSet() );
}

TEST_F( ActionTest, SubactionName )
{
  action.name() = "superaction";
  action.arguments() += Action( "action" );
  
  EXPECT_EQ( action.action( "action" ).name(), "superaction action" );
}

TEST_F( ActionTest, ActionEqualArgument )
{
  action.arguments() += Flag( "flag" );
  EXPECT_EXIT( action.arguments() += Action( "--flag" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, ArgumentEqualAction )
{
  action.arguments() += Action( "--flag" );
  EXPECT_EXIT( action.arguments() += Flag( "flag" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, ActionEqualShortName )
{
  action.arguments() += Flag( "flag", 'f' );
  EXPECT_EXIT( action.arguments() += Action( "-f" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, ShortNameEqualAction )
{
  action.arguments() += Action( "-f" );
  EXPECT_EXIT( action.arguments() += Flag( "flag", 'f' ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, PrintActions )
{ 
  string expected( "Usage:\n"\
                   "  action [ARGS] [ACTION ...]\n\n"\
                   "  Where ARGS are:\n"\
                   "\t--help              \tProducts this help message\n\n"\
                   "        ACTION is one of:\n"\
                   "\tact                  \tsome action\n\n"\
                   "  For help on concrete action\ttype: action ACTION --help\n\n" );
  
  action.name() = "action";
  Action subaction( "act", "some action" );
  action.arguments() += subaction;
  
  stringstream stream;
  action.printHelp( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( ActionTest, PositionalInsert )
{
  action.positionals() += Flag( "flag" );
  
  EXPECT_FALSE( action.positional( "flag" ).isSet() );
}

TEST_F( ActionTest, ExistingPositionalInsert )
{
  action.positionals() += Flag( "flag" );
  EXPECT_EXIT( action.positionals() += Flag( "flag" ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, NonExistingPositionalCheck )
{
  EXPECT_EXIT( action.positional( "flag" ).isSet(), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST_F( ActionTest, CommaSeparatedPositionalInsert )
{
  action.positionals() += Flag( "flag1" ), Flag( "flag2" );
  
  EXPECT_FALSE( action.positional( "flag1" ).isSet() );
  EXPECT_FALSE( action.positional( "flag2" ).isSet() );
}

TEST_F( ActionTest, PositionalHelp )
{
//                    "  Where key                 \tsome key\n"
//                          "\totherkey            \tsome other key\n\n"
  string expected( "Usage:\n"\
                   "  action [ARGS] [key] [otherkey]\n\n"\
                   "  Where key                  \tsome key\n"\
                         "\totherkey             \tsome other key\n"\
                   "\n        ARGS are:\n"\
                   "\t--help              \tProducts this help message\n\n" );
  
  action.name() = "action";
  action.positionals() += Key< int >( "key", "some key" );
  action.positionals() += Key< int >( "otherkey", "some other key" );
  
  stringstream stream;
  action.printHelp( stream );
  
  EXPECT_EQ( stream.str(), expected );
}
