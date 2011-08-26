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

#ifndef BURNING_COMMANDLINE_ARGUMENT_HPP
#define BURNING_COMMANDLINE_ARGUMENT_HPP
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/tr1/memory.hpp>
#include "ContainerTraits.hpp"
#include "Iterator.hpp"
#include "Check.hpp"

namespace burning
{
  namespace commandLine
  {
    class Argument;
    typedef std::tr1::shared_ptr< Argument > ArgumentPtr;
    
    /*! A command line's argument */
    class Argument
    {
    public:
      virtual ~Argument(){}
      /*! A pointer to clone of this argument */
      virtual ArgumentPtr clone() const = 0;
      
      /*! An argument's name */
      virtual const std::string& name() const = 0;
      /*! An argument's short name */
      virtual char shortName() const = 0;
      /*! An argument's description */
      virtual const std::string& description() const = 0;
      
      /*! Checks that argument is set */
      virtual bool isSet() const = 0;
      /*! Parses argument's value from command line */
      virtual void parse( Iterator& iterator ) = 0;
      
      /*! Converts value to a T type */
      template< class T >
      T as() const;
      
      /*! Checks that argument contains at least one value */
      virtual bool haveAnyValue() const = 0;
      /*! Checks that argument contains more than one value */
      virtual bool haveManyValues() const = 0;
    protected:
      
      /*! Converts containing value to a string vector */
      virtual std::vector< std::string > asStringVector() const = 0;
    };
    
    namespace impl
    {
      /*! A checked type implementaion for generic types */
      template< class T, bool isVector  >
      struct CheckedTypeImpl
      {
	/*! A checked type for simple type T.
	 *  It is a T.
	 */
	typedef T Type;
      };
      
      /*! A checked type struct for container types */
      template< class T >
      struct CheckedTypeImpl< T, true >
      {
	/*! A checked type for container type T.
	 *  It is a contained type.
	 */
	typedef typename Type< T >::ContainedType Type;
      };
      
      /*! A struct containing checked type for type T */
      template< class T >
      struct CheckedType
      {
	/*! A checked type for type T */
	typedef typename CheckedTypeImpl< T, Type< T >::IsContainer::value >::Type Type;
      };
    }
    
    /*! An arguments value */
    template< class T >
    class Value
    {
    public:
      Value();
      
      /*! Checked type for value */
      typedef typename impl::CheckedType< T >::Type CheckedType;
      /*! Parses value from command line and checks it. */
      bool parse( Iterator& iterator, const typename CheckPtr< CheckedType >::Type check );
      /*! Converts value to a string vector */
      std::vector< std::string > asStringVector() const;
      
      /*! True if value type has at least one value */
      static const bool haveAnyValue = true;
      /*! True if value type has more that one value */
      static const bool haveManyValues;
      
    private:
      bool parseImpl( Iterator& iterator, const Check< CheckedType >& check, FalseType );
      bool parseImpl( Iterator& iterator, const Check< CheckedType >& check, TrueType );
      
      std::vector< std::string > asStringVectorImpl( FalseType ) const;
      std::vector< std::string > asStringVectorImpl( TrueType ) const;
      
      T _value;
    };
    
    /*! A key command line argument */
    template< class T >
    class Key : public Argument
    {
    public:
      /*! A checked type for type T */
      typedef typename impl::CheckedType< T >::Type CheckedType;
      
      /*! Constructs key */
      Key();
      /*! Constructs key */
      Key( const std::string& name, const std::string& description = "", const Check< CheckedType >& check = TrueCheck< CheckedType >() );
      /*! Constructs key */
      Key( const std::string& name, char shortName, const std::string& description = "", const Check< CheckedType >& check = TrueCheck< CheckedType >() );
      /*! Constructs key */
      Key( const std::string& name, const Check< CheckedType >& check );
      /*! Constructs key from another */
      Key( const Key< T >& key );
      
      /*! Sets value of this key from another */
      void operator=( const Key< T >& key );
      
      const std::string& name() const;
      char shortName() const;
      const std::string& description() const;
      
      bool isSet() const;      
      void parse( Iterator& iterator );
      
      ArgumentPtr clone() const;
      
      bool haveAnyValue() const;
      bool haveManyValues() const;
    protected:
      
      /*! Converts containing value to vector of strings */
      std::vector< std::string > asStringVector() const;
    private:
      
      void set() { _isSet = true; };
      
      Value< T > _value;
      std::string _name;
      char _shortName;
      std::string _description;
      bool _isSet;
      
      typename CheckPtr< CheckedType >::Type _check;
    };
    
    namespace impl
    {
      /*! A type containing nothing */
      struct EmptyType
      {
      };
    }
    typedef Key< impl::EmptyType > Flag;
    
  }
}

#include "Value_impl.hpp"
#include "Key_impl.hpp"
#endif
