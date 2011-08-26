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

#include <boost/foreach.hpp>
#include "Node.hpp"

using std::string;
using std::istream;
using std::ostream;
using namespace burning::xml;

Node::Node( const string& name ): _name( name ), _parent( NULL )
{
}

NodePtr Node::create( const string& name )
{
  return NodePtr( new Node( name ));
}

string& Node::name()
{
  return _name;
}

const string& Node::name() const
{
  return _name;
}

void Node::write( ostream& stream, int indentation ) const
{
  stream << makeOpeningTag( indentation );
 
  if( _childs.size() > 0 )
  {
    BOOST_FOREACH( NodePtr node, childs() )
    {
      stream << std::endl;
      node->write( stream, indentation+1 );
    }
    stream << std::endl;
    stream << makeClosingTag( indentation );
  }
}

string Node::makeIndentation( int indentation )
{
  string ret;
  for( int i=0; i<indentation; i++ )
    ret += "  ";
  
  return ret;
}

string Node::makeOpeningTag( int indentation ) const
{
  string ret( makeIndentation( indentation ) );
  ret += "<"; 
  ret += _name;
  
  for( AttributeMap::const_iterator i = _attributes.begin(); i != _attributes.end(); i++ )
  {
    ret += ' ';
    ret += i->first;
    ret += '=';
    ret += i->second.toString();
  }
  
  if( _childs.size() == 0 )
    ret += "/>";
  else
    ret += ">";
  
  return ret;
}

string Node::makeClosingTag( int indentation ) const
{
  string ret( makeIndentation( indentation ));
  ret += "</";
  ret += _name;
  ret += '>';
  
  return ret;
}

AttributeProxy Node::attr( const string& name )
{
  return AttributeProxy( name, _attributes.find( name ), _attributes );
}

InsertableNodesProxy Node::childs()
{
  return InsertableNodesProxy( std::make_pair( _childs.begin(), _childs.end() ), *this );
}

const NodesProxy Node::childs() const
{
  return NodesProxy( std::make_pair( _childs.begin(), _childs.end() ));
}

NodesProxy Node::childs( const string& name )
{
  return NodesProxy( _childs.equal_range( create( name ) ));
}

Node* Node::parent()
{
  return _parent;
}

void Node::addChild( NodePtr child )
{
  child->_parent = this;
  _childs.insert( child );
}
