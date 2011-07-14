#include "AttributeProxy.hpp"

using std::ostream;
using std::string;
using namespace bng::xml;

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

ostream& bng::xml::operator<<( ostream& stream, const AttributeProxy& tag )
{
  if( tag._position == tag._map.end() ) 
    stream << "";
  else
    stream << tag._position->second.as< string >();
  
  return stream;
}
