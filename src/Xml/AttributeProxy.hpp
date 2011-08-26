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

#ifndef BURNING_XML_XMLATTRIBUTEPROXY_HPP
#define BURNING_XML_XMLATTRIBUTEPROXY_HPP

#include <map>
#include <ostream>
#include "Attribute.hpp"

namespace burning
{
  namespace xml
  {
    typedef std::map< std::string, Attribute > AttributeMap;
  
    /*! A proxy class representing node's attribute */
    class AttributeProxy
    {
    public:
      /*! Constructs proxy using specified attributes data */
      AttributeProxy( const std::string& name, AttributeMap::iterator position, AttributeMap& map );
    
      /*! Sets attributes value from existing attribute */
      void operator=( const Attribute& value );
      /*! Sets attributes value */
      void operator=( const Attribute::ValueType& value );
      /*! Equal operator for attribute */
      bool operator==( const Attribute::ValueType& value ) const;
      /*! NotEqual operator for attribute */
      bool operator!=( const Attribute::ValueType& value ) const;
    
      /*! Checks that attribute is set.*/
      bool isSet() const;
      /*! Writes attribute value to specidifed ostream */
      friend std::ostream& operator<<( std::ostream& ostream, const AttributeProxy& attribute );
    
      /*! A value of representing attribute */
      const Attribute::ValueType& value() const;
      template< class T >
      /*! Tries to represent attributes value as T type object. */
      T as()
      {
        if( _position == _map.end() ) 
  	  return T();
       return _position->second.as< T >();
      }
    
    private:
      std::string _name;
      AttributeMap::iterator _position;
      AttributeMap& _map;
      
    };
  
    std::ostream& operator<<( std::ostream& ostream, const AttributeProxy& attribute );
  }
}

#endif
