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

#include "Node.hpp"
#include "NodesProxy.hpp"

using std::pair;
using std::string;
using namespace burning::xml;

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
