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

#include <string>
#include "Attribute.hpp"

using std::string;
using std::ostream;
using namespace burning::xml;

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
 
ostream& burning::xml::operator<<( ostream& stream, const Attribute& attr )
{
  string value = boost::apply_visitor( Attribute::ToString(), attr._value );
  
  stream << value;
  return stream;
}
