#include "Node.hpp"
#include "NodesProxy.hpp"

using std::pair;
using std::string;
using namespace bng::xml;

NodesProxy::NodesProxy( const NodeRange& range ) : _range( range )
{
}

bool NodesProxy::exist() const
{
  return _range.first != _range.second;
}

size_t NodesProxy::count() const
{
  return std::distance( _range.first, _range.second );
}

InsertableNodesProxy::InsertableNodesProxy( const NodeRange& range, Node& node ): 
                                                       NodesProxy( range ), _node( node )
{
}

void InsertableNodesProxy::operator+=( const NodePtr& ptr )
{
  _node.addChild( ptr );
}

void InsertableNodesProxy::operator-=( const NodePtr& ptr )
{
  for( NodeSet::iterator iter = begin(); iter != end(); iter++ )
  {
    if( *iter == ptr )
      _node._childs.erase( iter );
  }
}

NodeSet::iterator NodesProxy::begin()
{
  return _range.first;
}

NodeSet::const_iterator NodesProxy::begin() const
{
  return _range.first;
}

NodeSet::iterator NodesProxy::end()
{
  return _range.second;
}

NodeSet::const_iterator NodesProxy::end() const
{
  return _range.second;
}