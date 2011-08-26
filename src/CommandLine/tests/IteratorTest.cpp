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
#include <CommandLine/Iterator.hpp>

using namespace burning::commandLine;

TEST( IteratorTest, EmptryLine )
{
  const char* argv[] = {};
  
  Iterator iter( 0, argv );
  
  EXPECT_TRUE( iter.atEnd() );
}

TEST( IteratorTest, Next )
{
  const char* argv[] = { "aaa" };
  
  Iterator iter( 1, argv );
  ASSERT_FALSE( iter.atEnd() );
  
  iter++;
  EXPECT_TRUE( iter.atEnd() );
}

TEST( IteratorTest, Prev )
{
  const char* argv[] = { "aaa", "bbb" };
  
  Iterator iter( 2, argv, 1 );
  EXPECT_EQ( *iter, "bbb" );
  iter--;
  EXPECT_EQ( *iter, "aaa" );
}

TEST( IteratorTest, PrefixNext )
{
  const char* argv[] = { "aaa" };
  
  Iterator iter( 1, argv );
  ASSERT_FALSE( iter.atEnd() );
  
  ++iter;
  EXPECT_TRUE( iter.atEnd() );
}

TEST( IteratorTest, Access )
{
  const char* argv[] = { "aaa" };
  
  Iterator iter( 1, argv );
  
  EXPECT_EQ( *iter, "aaa" );
}

TEST( IteratorTest, FailedAccess )
{
  const char* argv[] = {};
  
  Iterator iter( 0, argv );
  EXPECT_EXIT( *iter, testing::ExitedWithCode( EXIT_FAILURE ), "" );
}

TEST( IteratorTest, AtFlag )
{
  const char* argv[] = { "--html" };
  
  Iterator iter( 1, argv );
  
  EXPECT_TRUE( iter.atArgument() );
}

TEST( IteratorTest, AtShortFlag )
{
  const char* argv[] = { "-a" };
  
  Iterator iter( 1, argv );
  
  EXPECT_TRUE( iter.atArgument() );
}
