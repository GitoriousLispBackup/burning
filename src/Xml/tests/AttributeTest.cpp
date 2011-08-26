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
#include <Xml/Attribute.hpp>

using std::string;
using namespace burning::xml;

TEST( AttributeTest, IntAttribute )
{
  Attribute attr;
  attr = 123;
  
  EXPECT_TRUE( attr == 123 );
}

TEST( AttributeTest, StringAttribute )
{
  Attribute attr;
  attr = "value";
  
  EXPECT_TRUE( attr == "value" );
}

TEST( AttributeTest, FloatEQFail )
{
  Attribute attr;
  attr = 123;
  
  float value( 123.5 );
  EXPECT_FALSE( attr == value );
}

TEST( AttributeTest, FloatEQSuccess )
{
  Attribute attr;
  attr = 123;
  
  float value( 123 );
  EXPECT_EQ( attr, value );
}

TEST( AttributeTest, StringAs )
{
  Attribute attr;
  attr = "Hello";
  
  EXPECT_TRUE( attr.as< string >() == "Hello" ); 
  ASSERT_THROW( attr.as< int >(), boost::bad_lexical_cast );
}

TEST( AttributeTest, IntAs )
{
  Attribute attr;
  attr = 123;
  
  EXPECT_EQ( attr.as< int >(), 123 );
  EXPECT_EQ( attr.as< string >(), "123" );
}
