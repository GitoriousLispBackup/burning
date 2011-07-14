#include <string>
#include "XmlAttribute.hpp"

using std::string;
using std::ostream;
using namespace rtrt;

XmlAttribute::XmlAttribute() : _value()
{
}

void XmlAttribute::operator=( const ValueType& value )
{
  _value = value;
}

bool XmlAttribute::operator==( const ValueType& value ) const
{
  return _value == value;
}

class XmlAttribute::ToString : public boost::static_visitor< string >
{
public:
  string operator()( const string& value ) const
  {
    return "\"" + value + "\"";
  }
  string operator()( const Decimal& value ) const
  {
    return value.as< string >();
  }
};

string XmlAttribute::toString() const
{
  return boost::apply_visitor( XmlAttribute::ToString(), _value );
}
 
ostream& rtrt::operator<<( ostream& stream, const XmlAttribute& attr )
{
  string value = boost::apply_visitor( XmlAttribute::ToString(), attr._value );
  
  stream << value;
  return stream;
}