#include "XmlNode.hpp"
#include "XmlNodesProxy.hpp"

using std::pair;
using std::string;
using namespace rtrt;

XmlNodesProxy::XmlNodesProxy( const NodeRange& range ) : _range( range )
{
}

bool XmlNodesProxy::exist() const
{
  return _range.first != _range.second;
}

size_t XmlNodesProxy::count() const
{
  return std::distance( _range.first, _range.second );
}

InsertableXmlNodesProxy::InsertableXmlNodesProxy( const NodeRange& range, XmlNode& node ): 
                                                       XmlNodesProxy( range ), _node( node )
{
}

void InsertableXmlNodesProxy::operator+=( const XmlNodePtr& ptr )
{
  _node.addChild( ptr );
}

void InsertableXmlNodesProxy::operator-=( const XmlNodePtr& ptr )
{
  for( NodeSet::iterator iter = begin(); iter != end(); iter++ )
  {
    if( *iter == ptr )
      _node._childs.erase( iter );
  }
}

NodeSet::iterator XmlNodesProxy::begin()
{
  return _range.first;
}

NodeSet::const_iterator XmlNodesProxy::begin() const
{
  return _range.first;
}

NodeSet::iterator XmlNodesProxy::end()
{
  return _range.second;
}

NodeSet::const_iterator XmlNodesProxy::end() const
{
  return _range.second;
}