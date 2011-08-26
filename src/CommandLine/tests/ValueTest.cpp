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
#include <CommandLine/Argument.hpp>

using std::vector;
using namespace burning::commandLine;

TEST( ValueTest, FailedCheck )
{
  Flag flag( "flag", FalseCheck< impl::EmptyType >() );
  
  const char* argv[] = {};
  Iterator iter( 0, argv );
  
  EXPECT_EXIT( flag.parse( iter ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST( ValueTest, VectorCheck )
{
  const char* argv[] = { "1", "2", "3", "4" };
  Iterator iter( 4, argv );
  
  Key< vector< int > > key( "key", LessCheck< int >( 5 ) );
  key.parse( iter );
}

TEST( ValueTest, VectorFailedCheck )
{
  const char* argv[] = { "1", "2", "3", "4", "5" };
  Iterator iter( 5, argv );
  
  Key< vector< int > > key( "key", LessCheck< int >( 5 ) );
  EXPECT_EXIT( key.parse( iter ), testing::ExitedWithCode( EXIT_FAILURE ), "" );
}
