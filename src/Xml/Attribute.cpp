#include <string>
#include "Attribute.hpp"

using std::string;
using std::ostream;
using namespace bng::xml;

Attribute::Attribute() : _value()
{
}

void Attribute::operator=( const ValueType& value )
{
  _value = value;
}

bool Attribute::operator==( const ValueType& value ) const
{
  return _value == value;
}

class Attribute::ToString : public boost::static_visitor< string >
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

string Attribute::toString() const
{
  return boost::apply_visitor( Attribute::ToString(), _value );
}
 
ostream& bng::xml::operator<<( ostream& stream, const Attribute& attr )
{
  string value = boost::apply_visitor( Attribute::ToString(), attr._value );
  
  stream << value;
  return stream;
}