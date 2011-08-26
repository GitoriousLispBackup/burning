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
#include <sstream>
#include <Profiling/Table.hpp>

using std::string;
using std::stringstream;
using namespace burning::profiling;

class TableTest : public testing::Test
{
public:
  void SetUp();
  
  Table table;
  stringstream stream;
};
 
void TableTest::SetUp()
{
}

TEST_F( TableTest, InitializationTest )
{
  EXPECT_EQ( table.rows(), 0 );
  EXPECT_EQ( table.columns(), 0 );
}

TEST_F( TableTest, OneElementTest )
{
  string value( "SomeValue" );
  table[ 0 ][ 0 ] = value;
  
  EXPECT_EQ( table.rows(), 1 );
  EXPECT_EQ( table.columns(), 1 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "SomeValue" );
}

TEST_F( TableTest, RowAssignment )
{
  string value( "some string" );
  
  table.row( 0 )[ 0 ] = value;
  
  ASSERT_EQ( table.rows(), 1 );
  ASSERT_EQ( table.columns(), 1 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), value );
  
  ASSERT_EQ( table.row( 0 ).columns(), 1 );
  EXPECT_EQ( table.row( 0 )[ 0 ].value(), value );
}

TEST_F( TableTest, ColumnAssignment )
{
  string value( "value" );
  
  table.column( 0 )[ 0 ] = value;
  
  ASSERT_EQ( table.rows(), 1 );
  ASSERT_EQ( table.columns(), 1 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), value );
  
  ASSERT_EQ( table.column( 0 ).rows(), 1 );
  EXPECT_EQ( table.column( 0 )[ 0 ].value(), value );
}

TEST_F( TableTest, RowUnaligned )
{
  string value( "some value" );
  
  table.row( 7 )[ 3 ] = value;
  
  ASSERT_EQ( table.rows(), 8 );
  ASSERT_EQ( table.columns(), 4 );
  EXPECT_EQ( table[ 7 ][ 3 ].value(), value );
  EXPECT_EQ( table.row( 7 )[ 3 ].value(), value );
  
  EXPECT_EQ( table.row( 7 ).columns(), 4 );
  EXPECT_EQ( table.row( 5 ).columns(), 0 );
  EXPECT_EQ( table.column( 3 ).rows(), 8 );
  EXPECT_EQ( table.column( 4 ).rows(), 0 );
}

TEST_F( TableTest, ColumnUnaligned )
{
  string value( "a value" );
  
  table.column( 5 )[ 4 ] = value;
  
  ASSERT_EQ( table.columns(), 6 );
  ASSERT_EQ( table.rows(), 5 );
  EXPECT_EQ( table[ 4 ][ 5 ].value(), value );
  EXPECT_EQ( table.column( 5 )[ 4 ].value(), value );
  
  EXPECT_EQ( table.column( 5 ).rows(), 5 );
  EXPECT_EQ( table.column( 6 ).rows(), 0 );
  EXPECT_EQ( table.row( 4 ).columns(), 6 );
  EXPECT_EQ( table.row( 3 ).columns(), 0 );
}

TEST_F( TableTest, IntegerValue )
{
  int value( 42 );
  
  table[ 1 ][ 1 ] = value;
  
  EXPECT_EQ( table[ 1 ][ 1 ].value(), value );
}

TEST_F( TableTest, ColumnName )
{
  table[ 0 ][ 0 ] = "value";
  table.column( 0 ).name() = "first";
  
  EXPECT_EQ( table.columns(), 1 );
  EXPECT_EQ( table.rows(), 1 );
  EXPECT_EQ( table.column( 0 ).name(), "first" );
  EXPECT_EQ( table.column( "first" ).name(), "first" );
  EXPECT_EQ( table.column( "first" )[ 0 ].value(), "value" );
  EXPECT_EQ( table[ 0 ][ "first" ].value(), "value" );
}

TEST_F( TableTest, ColumnNameAssignment )
{
  table.column( "first" )[ 0 ] = "value";
  
  EXPECT_EQ( table.columns(), 1 );
  EXPECT_EQ( table.rows(), 1 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "value" );
}

TEST_F( TableTest, RowNameAssignment )
{
  table.row( 0 )[ "first" ] = "value";
  
  EXPECT_EQ( table.columns(), 1 );
  EXPECT_EQ( table.rows(), 1 );
  EXPECT_EQ( table[ 0 ][ "first" ].value(), "value" );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "value" );
}

TEST_F( TableTest, NewColumn )
{
  Table::ColumnProxy column( table.newColumn() );
  column[ 0 ] = "value";
  
  ASSERT_EQ( table.columns(), 1 );
  ASSERT_EQ( table.rows(), 1 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "value" );
}

TEST_F( TableTest, ColumnPushBack )
{
  table.newColumn().pushBack( "some value" );
  
  ASSERT_EQ( table.columns(), 1 );
  ASSERT_EQ( table.rows(), 1 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "some value" );
}

TEST_F( TableTest, DoubleColumnPushBack )
{
  Table::ColumnProxy column( table.newColumn() );
  
  column.pushBack( "aaa" );
  column.pushBack( "bbb" );
  
  ASSERT_EQ( table.columns(), 1 );
  ASSERT_EQ( table.rows(), 2 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "aaa" );
  EXPECT_EQ( table[ 1 ][ 0 ].value(), "bbb" );
}

TEST_F( TableTest, RowPushBack )
{
  table.newRow().pushBack( "some value" );
  
  ASSERT_EQ( table.columns(), 1 );
  ASSERT_EQ( table.rows(), 1 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "some value" );
}

TEST_F( TableTest, NewRow )
{
  Table::RowProxy row( table.newRow() );
  row[ 0 ] = "value";
  
  ASSERT_EQ( table.rows(), 1 );
  ASSERT_EQ( table.columns(), 1 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "value" );
}

TEST_F( TableTest, EmptyPrint )
{
  table.print( stream );
  
  EXPECT_EQ( stream.str(), "" );
}

TEST_F( TableTest, IntegerPrint )
{
  string expected( "1\t2\t\n"\
                   "3\t4\t\n" );
  
  table[ 0 ][ 0 ] = 1;
  table[ 0 ][ 1 ] = 2;
  table[ 1 ][ 0 ] = 3;
  table[ 1 ][ 1 ] = 4;
  
  table.print( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( TableTest, NamedColumnsPrint )
{
  string expected( "first\t\tthird\t\n"\
                   "1\t2\t3\t\n" );
  
  table[ 0 ][ 0 ] = 1;
  table[ 0 ][ 1 ] = 2;
  table[ 0 ][ 2 ] = 3;
  
  table.column( 0 ).name() = "first";
  table.column( 2 ).name() = "third";
  
  table.print( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( TableTest, MeasuredPrint )
{
  string expected( "first (fst)\t\n"\
                   "1\t\n" );
  
  table[ 0 ][ 0 ] = Value( 1, "fst" );
  table.column( 0 ).name() = "first";
  
  table.print( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( TableTest, DifferentMeasure )
{
  string expected( "first\tsecond (scd)\t\n"\
                   "1 (fst)\t2\t\n"\
                   "1 (scd)\t2\t\n" );
  
  table[ 0 ][ 0 ] = Value( 1, "fst" );
  table[ 1 ][ 0 ] = Value( 1, "scd" );
  table[ 0 ][ 1 ] = Value( 2, "scd" );
  table[ 1 ][ 1 ] = Value( 2, "scd" );
  
  table.column( 0 ).name() = "first";
  table.column( 1 ).name() = "second";
  
  table.print( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( TableTest, UnnamedMeasure )
{
  string expected( "1 (fst)\t2 (scd)\t\n" );
  
  table[ 0 ][ 0 ] = Value( 1, "fst" );
  table[ 0 ][ 1 ] = Value( 2, "scd" );
  
  table.print( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( TableTest, TwoColumnsBack )
{
  table.newColumn().pushBack( "value1" );
  table.newColumn().pushBack( "value2" );
  
  ASSERT_EQ( table.columns(), 2 );
  ASSERT_EQ( table.rows(), 1 );
  EXPECT_EQ( table[ 0 ][ 0 ].value(), "value1" );
  EXPECT_EQ( table[ 0 ][ 1 ].value(), "value2" );
}

TEST_F( TableTest, SimpleToHtml )
{
  string expected( "<table border=1>\n"\
                   "</table>\n" );
  
  table.printHtml( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( TableTest, IntegerToHtml )
{
  string expected( "<table border=1>\n"\
                   "<tr><td>1</td><td>2</td></tr>\n"\
                   "<tr><td>3</td><td>4</td></tr>\n"\
		   "</table>\n"
		 );
  
  table[ 0 ][ 0 ] = 1;
  table[ 0 ][ 1 ] = 2;
  table[ 1 ][ 0 ] = 3;
  table[ 1 ][ 1 ] = 4;
  
  table.printHtml( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( TableTest, NamedColumnsToHtml )
{
  string expected( "<table border=1>\n"\
                   "<tr><th>first</th><th></th><th>third</th></tr>\n"\
                   "<tr><td>1</td><td>2</td><td>3</td></tr>\n"\
		   "</table>\n"
		 );
  
  table[ 0 ][ 0 ] = 1;
  table[ 0 ][ 1 ] = 2;
  table[ 0 ][ 2 ] = 3;
  
  table.column( 0 ).name() = "first";
  table.column( 2 ).name() = "third";
  
  table.printHtml( stream );
  EXPECT_EQ( stream.str(), expected );
}
