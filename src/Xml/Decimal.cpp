#include "Decimal.hpp"

using std::string;
using std::ostream;
using boost::lexical_cast;
using namespace rtrt;

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

ostream& rtrt::operator<<( ostream& stream, const Decimal& decimal )
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
