#include <gtest/gtest.h>
#include <Xml/XmlAttribute.hpp>

using std::string;
using namespace rtrt;

TEST( XmlAttributeTest, IntAttribute )
{
  XmlAttribute attr;
  attr = 123;
  
  EXPECT_TRUE( attr == 123 );
}

TEST( XmlAttributeTest, StringAttribute )
{
  XmlAttribute attr;
  attr = "value";
  
  EXPECT_TRUE( attr == "value" );
}

TEST( XmlAttributeTest, FloatEQFail )
{
  XmlAttribute attr;
  attr = 123;
  
  float value( 123.5 );
  EXPECT_FALSE( attr == value );
}

TEST( XmlAttributeTest, FloatEQSuccess )
{
  XmlAttribute attr;
  attr = 123;
  
  float value( 123 );
  EXPECT_EQ( attr, value );
}

TEST( XmlAttributeTest, StringAs )
{
  XmlAttribute attr;
  attr = "Hello";
  
  EXPECT_TRUE( attr.as< string >() == "Hello" ); 
  ASSERT_THROW( attr.as< int >(), boost::bad_lexical_cast );
}

TEST( XmlAttributeTest, IntAs )
{
  XmlAttribute attr;
  attr = 123;
  
  EXPECT_EQ( attr.as< int >(), 123 );
  EXPECT_EQ( attr.as< string >(), "123" );
}