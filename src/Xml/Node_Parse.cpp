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

#include <glog/logging.h>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp> 
#include <boost/fusion/include/adapt_struct.hpp>
#include "Node.hpp"

using std::string;
using std::istream;
using namespace burning::xml;

namespace spirit = boost::spirit;
namespace phoenix = boost::phoenix;
namespace qi = spirit::qi;
namespace ascii = spirit::ascii;

struct setNameImpl
{
  template <typename Arg1, typename Arg2 >
  struct result
  {
      typedef void type;
  };

  template <typename Arg1, typename Arg2>
  void operator()(Arg1 node, Arg2 str ) const
  {
    node->name() = str;
  }
};
phoenix::function< setNameImpl > setName;

struct addTagImpl
{
  template <typename Arg1, typename Arg2 >
  struct result
  {
    typedef void type;
  };
  
  template <typename Arg1, typename Arg2>
  void operator()( Arg1 node, Arg2 attr ) const
  {
    node->attr( attr.name ) = attr.value;
  }
};
phoenix::function< addTagImpl > addTag;

struct addNodeImpl
{
  template <typename Arg1, typename Arg2 >
  struct result
  {
    typedef void type;
  };
  
  template< typename Arg1, typename Arg2 >
  void operator()( Arg1 node, Arg2 newNode ) const
  {
    node->childs() += newNode;
  }
};
phoenix::function< addNodeImpl > addNode;

struct TagType
{
  string name;
  Attribute::ValueType value;
};

BOOST_FUSION_ADAPT_STRUCT
(
  TagType,
  ( string, name )
  ( Attribute::ValueType, value )
)

template< class Iterator >
struct XmlGrammar : public qi::grammar< Iterator, NodePtr(), qi::locals< string >, ascii::space_type >
{
  XmlGrammar() : XmlGrammar::base_type( xml, "xml" )
  {
    
    using namespace qi::labels;
    using qi::eps;
    using qi::lexeme;
    using ascii::char_;
    using qi::_val;
    using qi::on_error;
    using qi::fail;
    using qi::int_;
    using qi::float_;
    using qi::double_;
    using qi::ulong_;
    using qi::lit;
    
    using phoenix::at_c;
    using phoenix::val; 
    using phoenix::construct;
    
    name %= lexeme[ +(char_ - '=' - '/' - ' ' - '>' ) ];
    
    quotedString %= lexeme['"' >> *(char_ - '"') >> '"']; 
    
    integer_ = lexeme
               [
                 ( int_
                   | ulong_ 
	         )
                 >> !lit( '.' )
               ][ _val = _1 ];
		
    tag %= !lit( '/' )
           >> !lit( '>' )
           > name
           > '='
           > ( integer_
               | double_
               | quotedString
	     );
	  
    node = xml[ _val = _1 ];
	
    closingTag = lit( "</" ) 
                 >> ascii::string( _r1 )
                 >> '>';
    
    xml = eps[ _val = phoenix::construct< NodePtr >( phoenix::new_< Node >() ) ] 
          >> '<' 
          >> name[ _a = _1 ]
          > eps[ setName( _val, _a ) ]
          > *tag[ addTag( _val, _1 ) ]
          > (  lit( "/>" )
	       | ( lit( ">" )
	           >> *node[ addNode( _val, _1 ) ]
	           >> closingTag( _a )
	         )
	    );
	  
    xml.name( "xml" ); 
    tag.name( "tag" );
    name.name( "name" );
    closingTag.name( "closing tag" );
    
    on_error<fail>
      ( xml, 
	std::cout
          << val("Error! Expecting ")
          << _4                               // what failed?
          << val(" here: \"")
          << construct<std::string>(_3, _2)   // iterators to error-pos, end
          << val("\"")
          << std::endl
      ); 
  }
  
  qi::rule< Iterator, NodePtr(), qi::locals< string >, ascii::space_type > xml;
  qi::rule< Iterator, NodePtr(), ascii::space_type > node;
  qi::rule< Iterator, string(), ascii::space_type > name;
  qi::rule< Iterator, TagType(), ascii::space_type > tag;
  qi::rule< Iterator, Attribute::ValueType(), ascii::space_type > integer_; 
  qi::rule< Iterator, string(), ascii::space_type > quotedString;
  qi::rule< Iterator, void( string ), ascii::space_type > closingTag;
};

NodePtr Node::parse( istream& stream )
{
  stream.unsetf(std::ios::skipws);
  
  spirit::istream_iterator begin( stream );
  spirit::istream_iterator end;
  
  XmlGrammar< spirit::istream_iterator > xmlParser;
  NodePtr ret;
  bool result = phrase_parse( begin, end, xmlParser, ascii::space, ret );
  if( !result || begin != end )
    return NodePtr();
  
  return ret;
}

