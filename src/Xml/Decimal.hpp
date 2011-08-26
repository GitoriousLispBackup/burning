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

#ifndef BURNING_XML_DECIMAL_HPP
#define BURNING_XML_DECIMAL_HPP

#include <boost/lexical_cast.hpp>
#include <string>

namespace burning
{
  /*! Class representing a general decimal value. */
  class Decimal
  {
  public:
    Decimal();
    /*! Constructs new decimal from integer value */
    Decimal( const int& value );
    /*! Constructs new decimal from size_t value */
    Decimal( const unsigned int& value );
    /*! Constructs new decimal from long value */
    Decimal( const long& value );
    /*! Constructs new decimal from unsigned long value */
    Decimal( const unsigned long& value );
    /*! Constructs new decimal from float value */
    Decimal( const float& value );
    /*! Constructs new decimal from double value */
    Decimal( const double& value );
    /*! Creates a copy of existing decimal object */
    Decimal( const Decimal& );
    
    /*! Copies a decimal object */
    Decimal& operator=( const Decimal& decimal );
    
    /*! Compares with other decimal object */
    bool operator==( const Decimal& value ) const;
    /*! Compares with integer value */
    bool operator==( const int& value ) const;
    /*! Compares with float value */
    bool operator==( const float& value ) const;
    /*! Compares with double value */
    bool operator==( const double& value ) const;
    /*! Compares with size_t value */
    bool operator==( const size_t& value ) const;
    
    /*! Checks that decimal object not equal integer value */
    bool operator!=( const int& value ) const;
    /*! Checks that decimal object not equal float value */
    bool operator!=( const float& value ) const;
    /*! Checks that decimal object not equal double value */
    bool operator!=( const double& value ) const;
    /*! Checks that decimal object not equal size_t value */
    bool operator!=( const size_t& value ) const;
    
    template< class T >
    /*! 
     * Tries to convert decimal value to type T. If failed throws boost::bad_lexical_cast exception.
     */
    T as() const
    {
      return boost::lexical_cast< T >( _value );
    }
    
    /*! Writes decimal's value to ostream */
    friend std::ostream& operator<<( std::ostream&, const Decimal& );
    
  private:
    template< class T >
    bool equal( const T& value ) const;
    template< class T >
    void set( const T& value );
    
    std::string _value;
  };
  
  std::ostream& operator<<( std::ostream&, const Decimal& );
}

#endif
