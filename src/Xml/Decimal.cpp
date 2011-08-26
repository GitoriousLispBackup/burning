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

#include "Decimal.hpp"

using std::string;
using std::ostream;
using boost::lexical_cast;
using namespace burning;

Decimal::Decimal() : _value()
{
}

Decimal::Decimal( const Decimal& decimal ) : _value( decimal._value )
{
}

Decimal& Decimal::operator=( const Decimal& decimal ) 
{
  _value = decimal._value;
  return *this;
}

bool Decimal::operator==( const Decimal& decimal ) const
{
  return _value == decimal._value;
}

template< class T >
void Decimal::set( const T& value )
{
  _value = lexical_cast< string >( value );
}

template< class T > 
bool Decimal::equal( const T& value ) const
{
  try
  {
    return lexical_cast< T >( _value ) == value;
  }
  catch( boost::bad_lexical_cast )
  {
    return false;
  }
}

ostream& burning::operator<<( ostream& stream, const Decimal& decimal )
{
  stream << decimal._value;
  return stream;
}

/*
 *   INT
 */

Decimal::Decimal( const int& value )
{
  set( value );
}

bool Decimal::operator==( const int& value ) const
{
  return equal( value );
}

bool Decimal::operator!=( const int& value ) const
{
  return !equal( value );
}

/*
 * FLOAT
 */

Decimal::Decimal( const float& value )
{
  set( value );
}

bool Decimal::operator==( const float& value ) const
{
  return equal( value );
}

bool Decimal::operator!=( const float& value ) const
{
  return !equal( value );
}

/*
 * DOUBLE
 */

Decimal::Decimal( const double& value )
{
  set( value );
}

bool Decimal::operator==( const double& value ) const
{
  return equal( value );
}

bool Decimal::operator!=( const double& value ) const
{
  return !equal( value );
}

/*
 *  SIZE_T
 */

Decimal::Decimal( const long& value )
{
  set( value );
}

Decimal::Decimal( const unsigned int& value )
{
  set( value );
}

Decimal::Decimal( const unsigned long& value )
{
  set( value );
}

bool Decimal::operator==( const size_t& value ) const
{
  return equal( value );
}

bool Decimal::operator!=( const size_t& value ) const
{
  return !equal( value );
}
