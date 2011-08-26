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

#include <boost/foreach.hpp>
#include "ContainerTraits.hpp"

namespace burning
{
  namespace commandLine
  {
    
    template< class T >
    const bool Value< T >::haveManyValues = Type< T >::IsContainer::value;
    
    template< class T >
    Value< T >::Value(): _value()
    {
    }
    
    template< class T >
    bool Value< T >::parse( Iterator& iterator, const typename CheckPtr< typename impl::CheckedType< T >::Type >::Type check )
    {
      return parseImpl( iterator, *check, typename Type< T >::IsContainer() );
    }
    
    template< class T >
    bool Value< T >::parseImpl( Iterator& iterator, const Check< CheckedType >& check, FalseType )
    {
      _value = boost::lexical_cast< T >( *iterator );
      iterator++;
      
      return check.check( _value );
    }
    
    template< class T >
    bool Value< T >::parseImpl( Iterator& iterator, const Check< CheckedType >& check, TrueType )
    {
      bool ret( true );
      
      while( !iterator.atEnd() && !iterator.atArgument() )
      {
	typename Type< T >::ContainedType value( boost::lexical_cast< typename Type< T >::ContainedType >( *iterator ) );
	if( !check.check( value ) )
	  ret = false;
	_value.insert( _value.end(), value );
	iterator++;
      }
      
      return ret;
    }
     
    template< class T >
    std::vector< std::string > Value< T >::asStringVector() const
    {
      return asStringVectorImpl( typename Type< T >::IsContainer() );
    }
    
    template< class T >
    std::vector< std::string > Value< T >::asStringVectorImpl( FalseType ) const
    {
      std::vector< std::string > ret;
      ret.push_back( boost::lexical_cast< std::string >( _value ) ); return ret;
    }
    
    template< class T >
    std::vector< std::string > Value< T >::asStringVectorImpl( TrueType ) const
    {
      std::vector< std::string > ret;
      BOOST_FOREACH( typename Type< T >::ContainedType value, _value )
      {
        ret.push_back( boost::lexical_cast< std::string >( value ) );
      }
      
      return ret;
    } 
    
    template<>
    class Value< impl::EmptyType >
    {
    public:
      Value(){};
      
      bool parse( Iterator& iterator, const CheckPtr< impl::EmptyType >::Type& check ){ return check->check( impl::EmptyType() ); };
      std::vector< std::string > asStringVector() const
      {
	return std::vector< std::string >();
      }
      
      static const bool haveAnyValue = false;
      static const bool haveManyValues = false;
      
      impl::EmptyType value()
      {
	return impl::EmptyType();
      }
    };
  }
}
