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
#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <utility>
#include <Xml/Node.hpp>

using std::string;
using std::stringstream;
using std::vector;
using namespace burning::xml;

class NodeTest : public testing::Test
{
public:
  void SetUp();
  
  stringstream stream;
};

void NodeTest::SetUp()
{
  stream.unsetf(std::ios::skipws);
}

TEST_F( NodeTest, EmptyInitialization )
{
  Node node;
  
  EXPECT_EQ( node.name(), "" );
}

TEST_F( NodeTest, Initialization )
{
  Node node( "node" );
  
  EXPECT_EQ( node.name(), "node" );
}

TEST_F( NodeTest, Assignment )
{
  Node node;
  node.name() = "node";
  
  EXPECT_EQ( node.name(), "node" );
}

TEST_F( NodeTest, SimpleOutput )
{
  string expected = "<node/>";
  Node node( "node" );
  
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( NodeTest, SimpleParse )
{
  stream.str( "<node/>" );
  
  NodePtr node( Node::parse( stream )); 
  
  ASSERT_FALSE( node == NULL );
  EXPECT_EQ( node->name(), "node" );
}

TEST_F( NodeTest, SimpleSpaceFail )
{
  stream.str( "<node f/>" );
  
  NodePtr node( Node::parse( stream ));
  
  ASSERT_TRUE( node == NULL );
}

TEST_F( NodeTest, NotExistingAttribute )
{
  Node node;
  
  EXPECT_FALSE( node.attr( "attr" ).isSet() );
  EXPECT_FALSE( node.attr( "attr" ).isSet() );
}

TEST_F( NodeTest, AddAttribute )
{
  Node node;
  node.attr( "attr" ) = "value";
  
  ASSERT_TRUE( node.attr( "attr" ).isSet() );
  EXPECT_EQ( node.attr( "attr" ), "value" );
}

TEST_F( NodeTest, EqNotExistingAttribute )
{
  Node node;
  
  EXPECT_NE( node.attr( "attr" ), "some value" );
}

TEST_F( NodeTest, ResetAttribute )
{
  Node node;
  node.attr( "attr" ) = "badValue";
  
  node.attr( "attr" ) = "fine";
  
  ASSERT_TRUE( node.attr( "attr" ).isSet() );
  EXPECT_EQ( node.attr( "attr" ), "fine" );
}

TEST_F( NodeTest, Integer )
{
  Node node;
  
  EXPECT_NE( node.attr( "attr" ), 123 );
  
  node.attr( "attr" ) = 123;
  
  ASSERT_TRUE( node.attr( "attr" ).isSet() );
  EXPECT_EQ( node.attr( "attr" ), 123 );
}

TEST_F( NodeTest, AsInt )
{
  Node node;
  node.attr( "attr" ) = 123;
  
  EXPECT_EQ( node.attr( "attr" ).as< int >(), 123 );
  EXPECT_EQ( node.attr( "attr" ).as< string >(), "123" );
}

TEST_F( NodeTest, IntAttributePrint )
{
  Node node( "node" );
  node.attr( "attr" ) = 123;
  
  string expected( "<node attr=123/>" );
  
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( NodeTest, TwoDecimalAttributesPrint )
{
  Node node( "node" );
  node.attr( "attr1" ) = 123;
  node.attr( "attr2" ) = 123.5;
  
  string expected( "<node attr1=123 attr2=123.5/>" );
  
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( NodeTest, StringAttributePrint )
{
  Node node( "node" );
  node.attr( "attr" ) = "quoted value";
  
  string expected( "<node attr=\"quoted value\"/>" );
  
  node.write( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( NodeTest, ParseIntAttribute )
{
  stream.str( "<node attr=123/>" );
  
  NodePtr node( Node::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr" ), 123 );
}

TEST_F( NodeTest, ParseSizeTAttribute )
{
  stream.str( "<node attr=1234567891/>" );
  
  NodePtr node( Node::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr" ), size_t( 1234567891 ));
}

TEST_F( NodeTest, ParseFloatAttribute )
{
  stream.str( "<node attr=123.5/>" );
  
  NodePtr node( Node::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr" ), 123.5F );
}

TEST_F( NodeTest, ParseStringAttribute )
{
  stream.str( "<node attr=\"quoted string\"/>" );
  
  NodePtr node( Node::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr" ), "quoted string" );
}

TEST_F( NodeTest, SeveralAttributesParse )
{
  stream.str( "<node attr1=123 attr2=123.5 attr3=\"quoted string\" />" );
  
  NodePtr node( Node::parse( stream ) );
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr1" ), 123 );
  EXPECT_EQ( node->attr( "attr2" ), 123.5 );
  EXPECT_EQ( node->attr( "attr3" ), "quoted string" );
}

TEST_F( NodeTest, ParseClosingTag )
{
  stream.str( "<node></node>");
  
  NodePtr node( Node::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->name(), "node" );
}

TEST_F( NodeTest, BadClosingTag )
{
  stream.str( "<node></notnode>" );
  
  NodePtr node( Node::parse( stream ));
  
  ASSERT_TRUE( node == NULL );
}

TEST_F( NodeTest, NoInnerNode )
{
  Node node;
  
  EXPECT_FALSE( node.childs( "child" ).exist() );
  EXPECT_FALSE( node.childs( "child" ).exist() );
  EXPECT_EQ( node.childs( "child" ).count(), 0 );
}

TEST_F( NodeTest, AddInnerNode )
{
  Node node;
  node.childs() += NodePtr( new Node( "child" ));
 
  EXPECT_TRUE( node.childs( "child" ).exist() );
  EXPECT_EQ( node.childs( "child" ).count(), 1 );
}

TEST_F( NodeTest, ChildsForeach )
{
  Node node;
  node.childs() += Node::create( "child1" );
  node.childs() += Node::create( "child2" );
  
  vector< string > names;
  BOOST_FOREACH( NodePtr val, node.childs() )
    names.push_back( val->name() );
  
  ASSERT_EQ( names.size(), 2 );
  EXPECT_EQ( names[ 0 ], "child1" );
  EXPECT_EQ( names[ 1 ], "child2" );
}

TEST_F( NodeTest, PartialChildsForeach )
{
  Node node;
  node.childs() += Node::create( "child1" );
  node.childs() += Node::create( "child1" );
  node.childs() += Node::create( "child2" );
  
  ASSERT_EQ( node.childs( "child1" ).count(), 2 );
  
  int count( 0 );
  BOOST_FOREACH( NodePtr curr, node.childs( "child1" ))
    count++;
    
  EXPECT_EQ( count, 2 );
}

TEST_F( NodeTest, InnerNodePrint )
{
  string expected( "<node1>\n"\
                   "  <node2/>\n"\
                   "</node1>" );
  
  Node node( "node1" );
  node.childs() += Node::create( "node2" );
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( NodeTest, ComplexInnerNodePrint )
{
  string expected( "<node1>\n"\
                   "  <node2/>\n"\
                   "  <node3>\n"\
                   "    <node4/>\n"\
                   "  </node3>\n"\
                   "  <node5/>\n"\
                   "</node1>" );
  
  Node node( "node1" );
  node.childs() += Node::create( "node2" );
  
  NodePtr node3( Node::create( "node3" ));
  node3->childs() += Node::create( "node4" );
  
  node.childs() += node3;
  node.childs() += Node::create( "node5" );
  
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( NodeTest, EmptyParentTest )
{
  Node node;
  
  EXPECT_TRUE( node.parent() == NULL );
}

TEST_F( NodeTest, ParentTest )
{
  Node node;
  NodePtr node2( Node::create());
  node.childs() += node2;
  
  EXPECT_EQ( node2->parent(), &node );
}

TEST_F( NodeTest, InnerNodeParse )
{
  stream.str( "<node1><node2/></node1>" );
  
  NodePtr node( Node::parse( stream ));
  
  ASSERT_FALSE( node == NULL );
  EXPECT_EQ( node->name(), "node1" );
  EXPECT_EQ( node->childs().count(), 1 );
  EXPECT_EQ( node->childs( "node2" ).count(), 1 );
}

TEST_F( NodeTest, ComplexNodeParse )
{
  stream.str( "<node1>\n"\
              "  <node2/>\n"\
              "  <node3>\n"\
              "    <node4/>\n"\
              "    <node5/>\n"\
              "  </node3>\n"\
              "</node1>" );
  
  NodePtr node( Node::parse( stream ));
  
  ASSERT_FALSE( node == NULL );
  EXPECT_EQ( node->name(), "node1" );
  EXPECT_EQ( node->childs().count(), 2 );
  EXPECT_EQ( node->childs( "node2" ).count(), 1 );
  EXPECT_EQ( node->childs( "node3" ).count(), 1 );
}
