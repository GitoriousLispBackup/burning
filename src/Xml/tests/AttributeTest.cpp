#include <gtest/gtest.h>
#include <Xml/Attribute.hpp>

using std::string;
using namespace bng::xml;

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