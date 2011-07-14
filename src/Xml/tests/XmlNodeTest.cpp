#include <sstream>
#include <boost/foreach.hpp>
#include <gtest/gtest.h>
#include <utility>
#include <Xml/XmlNode.hpp>

using std::string;
using std::stringstream;
using std::vector;
using namespace rtrt;

class XmlNodeTest : public testing::Test
{
public:
  void SetUp();
  
  stringstream stream;
};

void XmlNodeTest::SetUp()
{
  stream.unsetf(std::ios::skipws);
}

TEST_F( XmlNodeTest, EmptyInitialization )
{
  XmlNode node;
  
  EXPECT_EQ( node.name(), "" );
}

TEST_F( XmlNodeTest, Initialization )
{
  XmlNode node( "node" );
  
  EXPECT_EQ( node.name(), "node" );
}

TEST_F( XmlNodeTest, Assignment )
{
  XmlNode node;
  node.name() = "node";
  
  EXPECT_EQ( node.name(), "node" );
}

TEST_F( XmlNodeTest, SimpleOutput )
{
  string expected = "<node/>";
  XmlNode node( "node" );
  
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( XmlNodeTest, SimpleParse )
{
  stream.str( "<node/>" );
  
  XmlNodePtr node( XmlNode::parse( stream )); 
  
  ASSERT_FALSE( node == NULL );
  EXPECT_EQ( node->name(), "node" );
}

TEST_F( XmlNodeTest, SimpleSpaceFail )
{
  stream.str( "<node f/>" );
  
  XmlNodePtr node( XmlNode::parse( stream ));
  
  ASSERT_TRUE( node == NULL );
}

TEST_F( XmlNodeTest, NotExistingAttribute )
{
  XmlNode node;
  
  EXPECT_FALSE( node.attr( "attr" ).isSet() );
  EXPECT_FALSE( node.attr( "attr" ).isSet() );
}

TEST_F( XmlNodeTest, AddAttribute )
{
  XmlNode node;
  node.attr( "attr" ) = "value";
  
  ASSERT_TRUE( node.attr( "attr" ).isSet() );
  EXPECT_EQ( node.attr( "attr" ), "value" );
}

TEST_F( XmlNodeTest, EqNotExistingAttribute )
{
  XmlNode node;
  
  EXPECT_NE( node.attr( "attr" ), "some value" );
}

TEST_F( XmlNodeTest, ResetAttribute )
{
  XmlNode node;
  node.attr( "attr" ) = "badValue";
  
  node.attr( "attr" ) = "fine";
  
  ASSERT_TRUE( node.attr( "attr" ).isSet() );
  EXPECT_EQ( node.attr( "attr" ), "fine" );
}

TEST_F( XmlNodeTest, Integer )
{
  XmlNode node;
  
  EXPECT_NE( node.attr( "attr" ), 123 );
  
  node.attr( "attr" ) = 123;
  
  ASSERT_TRUE( node.attr( "attr" ).isSet() );
  EXPECT_EQ( node.attr( "attr" ), 123 );
}

TEST_F( XmlNodeTest, AsInt )
{
  XmlNode node;
  node.attr( "attr" ) = 123;
  
  EXPECT_EQ( node.attr( "attr" ).as< int >(), 123 );
  EXPECT_EQ( node.attr( "attr" ).as< string >(), "123" );
}

TEST_F( XmlNodeTest, IntAttributePrint )
{
  XmlNode node( "node" );
  node.attr( "attr" ) = 123;
  
  string expected( "<node attr=123/>" );
  
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( XmlNodeTest, TwoDecimalAttributesPrint )
{
  XmlNode node( "node" );
  node.attr( "attr1" ) = 123;
  node.attr( "attr2" ) = 123.5;
  
  string expected( "<node attr1=123 attr2=123.5/>" );
  
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( XmlNodeTest, StringAttributePrint )
{
  XmlNode node( "node" );
  node.attr( "attr" ) = "quoted value";
  
  string expected( "<node attr=\"quoted value\"/>" );
  
  node.write( stream );
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( XmlNodeTest, ParseIntAttribute )
{
  stream.str( "<node attr=123/>" );
  
  XmlNodePtr node( XmlNode::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr" ), 123 );
}

TEST_F( XmlNodeTest, ParseSizeTAttribute )
{
  stream.str( "<node attr=1234567891/>" );
  
  XmlNodePtr node( XmlNode::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr" ), size_t( 1234567891 ));
}

TEST_F( XmlNodeTest, ParseFloatAttribute )
{
  stream.str( "<node attr=123.5/>" );
  
  XmlNodePtr node( XmlNode::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr" ), 123.5F );
}

TEST_F( XmlNodeTest, ParseStringAttribute )
{
  stream.str( "<node attr=\"quoted string\"/>" );
  
  XmlNodePtr node( XmlNode::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr" ), "quoted string" );
}

TEST_F( XmlNodeTest, SeveralAttributesParse )
{
  stream.str( "<node attr1=123 attr2=123.5 attr3=\"quoted string\" />" );
  
  XmlNodePtr node( XmlNode::parse( stream ) );
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->attr( "attr1" ), 123 );
  EXPECT_EQ( node->attr( "attr2" ), 123.5 );
  EXPECT_EQ( node->attr( "attr3" ), "quoted string" );
}

TEST_F( XmlNodeTest, ParseClosingTag )
{
  stream.str( "<node></node>");
  
  XmlNodePtr node( XmlNode::parse( stream ));
  
  ASSERT_TRUE( node != NULL );
  EXPECT_EQ( node->name(), "node" );
}

TEST_F( XmlNodeTest, BadClosingTag )
{
  stream.str( "<node></notnode>" );
  
  XmlNodePtr node( XmlNode::parse( stream ));
  
  ASSERT_TRUE( node == NULL );
}

TEST_F( XmlNodeTest, NoInnerNode )
{
  XmlNode node;
  
  EXPECT_FALSE( node.childs( "child" ).exist() );
  EXPECT_FALSE( node.childs( "child" ).exist() );
  EXPECT_EQ( node.childs( "child" ).count(), 0 );
}

TEST_F( XmlNodeTest, AddInnerNode )
{
  XmlNode node;
  node.childs() += XmlNodePtr( new XmlNode( "child" ));
 
  EXPECT_TRUE( node.childs( "child" ).exist() );
  EXPECT_EQ( node.childs( "child" ).count(), 1 );
}

TEST_F( XmlNodeTest, ChildsForeach )
{
  XmlNode node;
  node.childs() += XmlNode::create( "child1" );
  node.childs() += XmlNode::create( "child2" );
  
  vector< string > names;
  BOOST_FOREACH( XmlNodePtr val, node.childs() )
    names.push_back( val->name() );
  
  ASSERT_EQ( names.size(), 2 );
  EXPECT_EQ( names[ 0 ], "child1" );
  EXPECT_EQ( names[ 1 ], "child2" );
}

TEST_F( XmlNodeTest, PartialChildsForeach )
{
  XmlNode node;
  node.childs() += XmlNode::create( "child1" );
  node.childs() += XmlNode::create( "child1" );
  node.childs() += XmlNode::create( "child2" );
  
  ASSERT_EQ( node.childs( "child1" ).count(), 2 );
  
  int count( 0 );
  BOOST_FOREACH( XmlNodePtr curr, node.childs( "child1" ))
    count++;
    
  EXPECT_EQ( count, 2 );
}

TEST_F( XmlNodeTest, InnerNodePrint )
{
  string expected( "<node1>\n"\
                   "  <node2/>\n"\
                   "</node1>" );
  
  XmlNode node( "node1" );
  node.childs() += XmlNode::create( "node2" );
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( XmlNodeTest, ComplexInnerNodePrint )
{
  string expected( "<node1>\n"\
                   "  <node2/>\n"\
                   "  <node3>\n"\
                   "    <node4/>\n"\
                   "  </node3>\n"\
                   "  <node5/>\n"\
                   "</node1>" );
  
  XmlNode node( "node1" );
  node.childs() += XmlNode::create( "node2" );
  
  XmlNodePtr node3( XmlNode::create( "node3" ));
  node3->childs() += XmlNode::create( "node4" );
  
  node.childs() += node3;
  node.childs() += XmlNode::create( "node5" );
  
  node.write( stream );
  
  EXPECT_EQ( stream.str(), expected );
}

TEST_F( XmlNodeTest, EmptyParentTest )
{
  XmlNode node;
  
  EXPECT_TRUE( node.parent() == NULL );
}

TEST_F( XmlNodeTest, ParentTest )
{
  XmlNode node;
  XmlNodePtr node2( XmlNode::create());
  node.childs() += node2;
  
  EXPECT_EQ( node2->parent(), &node );
}

TEST_F( XmlNodeTest, InnerNodeParse )
{
  stream.str( "<node1><node2/></node1>" );
  
  XmlNodePtr node( XmlNode::parse( stream ));
  
  ASSERT_FALSE( node == NULL );
  EXPECT_EQ( node->name(), "node1" );
  EXPECT_EQ( node->childs().count(), 1 );
  EXPECT_EQ( node->childs( "node2" ).count(), 1 );
}

TEST_F( XmlNodeTest, ComplexNodeParse )
{
  stream.str( "<node1>\n"\
              "  <node2/>\n"\
              "  <node3>\n"\
              "    <node4/>\n"\
              "    <node5/>\n"\
              "  </node3>\n"\
              "</node1>" );
  
  XmlNodePtr node( XmlNode::parse( stream ));
  
  ASSERT_FALSE( node == NULL );
  EXPECT_EQ( node->name(), "node1" );
  EXPECT_EQ( node->childs().count(), 2 );
  EXPECT_EQ( node->childs( "node2" ).count(), 1 );
  EXPECT_EQ( node->childs( "node3" ).count(), 1 );
}