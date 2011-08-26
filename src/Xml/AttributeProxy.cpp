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

#include "AttributeProxy.hpp"

using std::ostream;
using std::string;
using namespace burning::xml;

AttributeProxy::AttributeProxy( const string& name, AttributeMap::iterator position, AttributeMap& map ): _name( name ), _position( position ), _map( map )
{
}

void AttributeProxy::operator=( const Attribute& value )
{
  _map[ _name ] = value;
}

void AttributeProxy::operator=( const Attribute::ValueType& value )
{
  _map[ _name ] = value;
}

bool AttributeProxy::operator==( const Attribute::ValueType& value ) const
{
  if( _position == _map.end() )
    return false;
  return _position->second == value;
}

bool AttributeProxy::operator!=( const Attribute::ValueType& value ) const
{
  return !( *this == value ); 
}

bool AttributeProxy::isSet() const
{
  return _position != _map.end();
}

const Attribute::ValueType& AttributeProxy::value() const
{
  return _position->second.value();
}

ostream& burning::xml::operator<<( ostream& stream, const AttributeProxy& tag )
{
  if( tag._position == tag._map.end() ) 
    stream << "";
  else
    stream << tag._position->second.as< string >();
  
  return stream;
}
