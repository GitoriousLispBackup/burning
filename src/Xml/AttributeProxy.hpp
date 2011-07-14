#ifndef RTRT_XML_XMLATTRIBUTEPROXY_HPP
#define RTRT_XML_XMLATTRIBUTEPROXY_HPP

#include <map>
#include <ostream>
#include "Attribute.hpp"

namespace bng
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