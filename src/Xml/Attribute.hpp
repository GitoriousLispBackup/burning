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

#ifndef BURNING_XML_ATTRIBUTE_HPP
#define BURNING_XML_ATTRIBUTE_HPP
#include <boost/variant.hpp>
#include "Decimal.hpp"

namespace burning
{
  namespace xml
  {
    /*! Representing attribute of an XML node */
    class Attribute
    {
    public:
      /*! A type of value for attribute. May be a string or a some decimal value. */
      typedef boost::variant< Decimal, std::string > ValueType;
    
      Attribute();
      /*! Constructs new attribute as a copy of existing one  */
      Attribute( const ValueType& value ): _value( value )
      {
      }
    
      /*! Sets attributes value */
      void operator=( const ValueType& value );
      /*! Equal operator for attribute's value */
      bool operator==( const ValueType& value ) const;
    
      /*! A value of attribute */
      const ValueType& value()
      {
        return _value;
      }
    
      template< class T >
      /*! Converts value to T type. If fails throws boost::bad_lexical_cast exception. */
      T as() const
      {
        return boost::lexical_cast< T >( _value );
      }
    
      /*! Writes attribute's value to ostream */
      friend std::ostream& operator<<( std::ostream& stream, const Attribute& tag );
    
      /*! Converts attribute's value to string */
      std::string toString() const;
    
    private:
      ValueType _value;
    
      class ToString;
    };
  
    std::ostream& operator<<( std::ostream& stream, const Attribute& tag );
  }
 }

#endif
