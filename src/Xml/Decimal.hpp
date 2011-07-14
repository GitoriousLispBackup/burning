#ifndef RTRT_XML_DECIMAL_HPP
#define RTRT_XML_DECIMAL_HPP

#include <boost/lexical_cast.hpp>
#include <string>

namespace rtrt
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