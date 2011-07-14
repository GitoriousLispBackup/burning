#include "XmlAttributeProxy.hpp"

using std::ostream;
using std::string;
using namespace rtrt;

XmlAttributeProxy::XmlAttributeProxy( const string& name, AttributeMap::iterator position, AttributeMap& map ): _name( name ), _position( position ), _map( map )
{
}

void XmlAttributeProxy::operator=( const XmlAttribute& value )
{
  _map[ _name ] = value;
}

void XmlAttributeProxy::operator=( const XmlAttribute::ValueType& value )
{
  _map[ _name ] = value;
}

bool XmlAttributeProxy::operator==( const XmlAttribute::ValueType& value ) const
{
  if( _position == _map.end() )
    return false;
  return _position->second == value;
}

bool XmlAttributeProxy::operator!=( const XmlAttribute::ValueType& value ) const
{
  return !( *this == value ); 
}

bool XmlAttributeProxy::isSet() const
{
  return _position != _map.end();
}

const XmlAttribute::ValueType& XmlAttributeProxy::value() const
{
  return _position->second.value();
}

ostream& rtrt::operator<<( ostream& stream, const XmlAttributeProxy& tag )
{
  if( tag._position == tag._map.end() ) 
    stream << "";
  else
    stream << tag._position->second.as< string >();
  
  return stream;
}
