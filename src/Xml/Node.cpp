#include <boost/foreach.hpp>
#include "Node.hpp"

using std::string;
using std::istream;
using std::ostream;
using namespace bng::xml;

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