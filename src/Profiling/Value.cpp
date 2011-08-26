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

#include "Value.hpp"

using std::string;
using namespace burning::profiling;

Value::Value(): _value(),
                _measure()
{
}

Value::Value( const Value& value 
            ): _value( value._value ),
               _measure( value._measure )
{
}
      
Value& Value::operator=( const Value& value )
{
  _value = value._value;
  _measure = value._measure;

  return *this;
}
      
const std::string& Value::measure() const
{
  return _measure;
}

burning::xml::Attribute Value::value() const
{
 return _value;
}
 
