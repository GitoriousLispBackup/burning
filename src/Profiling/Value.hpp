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

#ifndef BURNING_PROFILING_VALUE_HPP
#define BURNING_PROFILING_VALUE_HPP

#include <string>
#include <vector>
#include <boost/variant.hpp>
#include <Xml/Attribute.hpp>

namespace burning
{
  namespace profiling
  {
    
    /*! Class representing value in profiling result */
    class Value
    {
    public:
      Value();
      
      /*! Constructs new value from given value and measure 
       *\param value A representing value
       *\param measure A name of measure for value
       */
      template< class T >
      Value( const T& value, const std::string& measure = "" ):
             _value( value ),
             _measure( measure )
      {
      }
      
      /*! Constructs new value as a copy of existing one. */
      Value( const Value& value );
      
      /*! Copies value from an existing one */
      Value& operator=( const Value& value );
      
      /*! Name of measure for value */
      const std::string& measure() const;
      /*! A representing attribute */
      xml::Attribute value() const;
      
      /*! A representing value */
      const xml::Attribute::ValueType& rawValue() const
      {
	return _value;
      }
      
    private:
      xml::Attribute::ValueType _value;
      std::string _measure;
    };
    
  }
}
#endif
