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
#include <Xml/Decimal.hpp>

using namespace burning;

TEST( DecimalTest, Int )
{
  Decimal d( 123 );
  
  EXPECT_EQ( d, 123 );
  EXPECT_NE( d, 132 );
}

TEST( DecimalTest, LargeInt )
{
  Decimal d( 1234567 );
  
  EXPECT_EQ( d, 1234567 );
}

TEST( DecimalTest, Float )
{
  Decimal d( 123.F );
  
  EXPECT_EQ( d, 123.F );
  EXPECT_NE( d, 132.F );
}

TEST( DecimalTest, Double )
{
  Decimal d( 123.4 );
  
  EXPECT_EQ( d, 123.4 );
  EXPECT_NE( d, 132.4 );
  EXPECT_NE( d, 123 );
}

TEST( DecimalTest, DoubleToInterger )
{
  Decimal d( 123.0 );
  
  EXPECT_EQ( d, 123 );
}

TEST( DecimalTest, SizeT )
{
  Decimal d( 123456789 );
  
  EXPECT_EQ( d, 123456789 );
  EXPECT_NE( d, 123456798 );
  EXPECT_NE( d, 12345 );
}

TEST( DecimalTest, SizeT2 )
{
  size_t value = 123456789;
  Decimal d( value );
  
  EXPECT_EQ( d, value );
  EXPECT_NE( d, value+1 );
}

TEST( DecimalTest, LittleSizeT )
{
  size_t value( 123 );
  Decimal d( value );
  
  EXPECT_EQ( d, 123 );
  EXPECT_EQ( d, value );
}

TEST( DecimalTest, IntAs)
{
  Decimal d( 12345 );
  
  EXPECT_EQ( d.as< int >(), 12345 );
  EXPECT_EQ( d.as< float >(), 12345.0F );
}

TEST( DecimalTest, FloatAs )
{
  Decimal d( 1234.56 );
  
  EXPECT_FLOAT_EQ( d.as< float >(), 1234.56 );
}

TEST( DecimalTest, FloatAsInt )
{
  Decimal d( 1234.F );
  
  EXPECT_EQ( d.as< float >(), 1234 );
  EXPECT_EQ( d.as< int >(), 1234 );
}
