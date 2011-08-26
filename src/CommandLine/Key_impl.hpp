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

#include <glog/logging.h>

namespace burning
{
  namespace commandLine
  {
    namespace impl
    {
      template< class T >
      T asImpl( const std::vector< std::string >& vector, FalseType )
      {
	assert( vector.size() == 1 );
	return boost::lexical_cast< T >( vector[ 0 ] );
      }
      
      template< class T >
      T asImpl( const std::vector< std::string >& vector, TrueType )
      {
	T ret;
	
	BOOST_FOREACH( std::string value, vector )
	  ret.insert( ret.end(), boost::lexical_cast< typename Type< T >::ContainedType >( value ) ); 
	  
	return ret;
      }
       
    }
    
    template< class T >
    T Argument::as() const
    {
      if( !haveAnyValue() )
      {
	LOG( ERROR ) << "Tried to convert void to type.";
	exit( EXIT_FAILURE );
      }
      
      std::vector< std::string > value( asStringVector() );
      return impl::asImpl< T >( value, typename Type< T >::IsContainer() );
    }
    
    template< class T >
    Key< T >::Key() : _value(),
                      _name(),
                      _shortName( '\0' ),
                      _description(),
                      _isSet( false ),
                      _check( new TrueCheck< CheckedType >() )
    {
    }
    
    template< class T >
    Key< T >::Key( const std::string& name, const std::string& description, const Check< CheckedType >& check ):
                   _value(),
                   _name( name ),
                   _shortName( '\0' ),
                   _description( description ),
                   _isSet( false ),
                   _check( check.clone() )
    {
    }
    
    template< class T >
    Key< T >::Key( const std::string& name, char shortName, const std::string& description, const Check< CheckedType >& check ):
                   _value(),
                   _name( name ),
                   _shortName( shortName ),
                   _description( description ),
                   _isSet( false ),
                   _check( check.clone() )
    {
    }
    
    template< class T >
    Key< T >::Key( const std::string& name, const Check< CheckedType >& check ):
                   _value(),
                   _name( name ),
                   _shortName( '\0' ),
                   _description(),
                   _isSet( false ),
                   _check( check.clone() )
    {
    }
   
    template< class T >
    Key< T >::Key( const Key< T >& key ):
                   _value( key._value ),
                   _name( key._name ),
                   _shortName( key._shortName ),
                   _description( key._description ),
                   _isSet( key._isSet ),
                   _check( key._check )
    {
    }
   
    template< class T >
    void Key< T >::operator=( const Key< T >& key )
    {
      _value = key._value;
      _name = key._name;
      _shortName = key._shortName;
      _description = key._description;
      _isSet = key._isSet;
      _check = key._check;
    }
   
    template< class T >
    const std::string& Key< T >::name() const
    {
      return _name;
    }
   
    template< class T >
    char Key< T >::shortName() const
    {
      return _shortName;
    }
  
    template< class T >
    const std::string& Key< T >::description() const
    {
      return _description;
    }
    
    template< class T >
    bool Key< T >::isSet() const
    {
      return _isSet;
    }
   
    template< class T >
    void Key< T >::parse( Iterator& iterator )
    {
      set();
      if( !_value.parse( iterator, _check ) )
      {
	LOG( ERROR ) << "Check for key " << name() << " failed.";
	exit( EXIT_FAILURE );
      }
    }
    
    template< class T >
    ArgumentPtr Key< T >::clone() const
    {
      return ArgumentPtr( new Key( *this ) );
    }
   
    template< class T >
    bool Key< T >::haveAnyValue() const
    {
     return Value< T >::haveAnyValue;
    }
    
    template< class T >
    bool Key< T >::haveManyValues() const
    {
      return Value< T >::haveManyValues;
    }
    
    template< class T >
    std::vector< std::string > Key< T >::asStringVector() const
    {
      return _value.asStringVector();
    }
  }
}
