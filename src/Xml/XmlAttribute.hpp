#ifndef RTRT_XML_XMLATTRIBUTE_HPP
#define RTRT_XML_XMLATTRIBUTE_HPP
#include <boost/variant.hpp>
#include "Decimal.hpp"

namespace rtrt
{
  /*! Representing attribute of an XML node */
  class XmlAttribute
  {
  public:
    /*! A type of value for attribute. May be a string or a some decimal value. */
    typedef boost::variant< Decimal, std::string > ValueType;
    
    XmlAttribute();
    /*! Constructs new attribute as a copy of existing one  */
    XmlAttribute( const ValueType& value ): _value( value )
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
    friend std::ostream& operator<<( std::ostream& stream, const XmlAttribute& tag );
    
    /*! Converts attribute's value to string */
    std::string toString() const;
    
  private:
    ValueType _value;
    
    class ToString;
  };
  
  std::ostream& operator<<( std::ostream& stream, const XmlAttribute& tag );
 }

#endif