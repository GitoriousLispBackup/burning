#include <gtest/gtest.h>
#include <Xml/Decimal.hpp>

using namespace bng;

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