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

#ifndef BURNING_COMMANDLINE_CHECK_HPP
#define BURNING_COMMANDLINE_CHECK_HPP

#include <boost/tr1/memory.hpp>
namespace burning
{
  namespace commandLine
  {
    template< class T >
    class Check;
    template< class T >
    class AndCheck;
    template< class T >
    class OrCheck;
    template< class T >
    class NotCheck;
    
    /*! Implementaion of template typedef for smart poiner to Check */
    template< class T >
    struct CheckPtr
    {
      /*! A smart pointer to check type */
      typedef std::tr1::shared_ptr< Check< T > > Type;
    };
    
    /*! Base class for value checkers */
    template< class T >
    class Check
    {
    public:
      virtual ~Check(){};
      
      /*! Gets pointer to clone of curent check */
      virtual typename CheckPtr< T >::Type clone() const = 0;
      /*! Check correctness of value */
      virtual bool check( const T& value ) const = 0;
      
      /*! Constructing and check using two specified checks */
      AndCheck< T > operator&&( const Check< T >& check ) 
      {
	return AndCheck< T >( *this, check );
      }
      
      /*! Constructing or check using two specified checks */
      OrCheck< T > operator||( const Check< T >& check )
      {
	return OrCheck< T >( *this, check );
      }
      
      /*! A check negating current */
      NotCheck< T > operator!()
      {
	return NotCheck< T >( *this );
      }
    };
    
    namespace check
    {
      /*! A base class for cloneale checks */
      template< template< class > class U, class T >
      class Clonable : public Check< T >
      {
      public:
	/*! Implementation of clone function for check classes inherting Clonable.
	 *  Uses curiously recurring template pattern.
	 */
	typename CheckPtr< T >::Type clone() const
	{
	  return typename CheckPtr< T >::Type( new U< T >( static_cast< const U< T >& >( *this ) ) );
	}
      };
    }
    
    /*! Always true check */
    template< class T >
    class TrueCheck : public check::Clonable< TrueCheck, T >
    {
    public:
      bool check( const T& value ) const
      {
	return true;
      }
    };
    
    /*! Always false check */
    template< class T >
    class FalseCheck : public check::Clonable< FalseCheck, T >
    {
    public:
      bool check( const T& value ) const
      {
	return false;
      }
    };
    
    /*! Chechs that both containing checks are true */
    template< class T >
    class AndCheck : public check::Clonable< AndCheck, T >
    {
    public:
      /*! Constructs check from operand check objects */
      AndCheck( const Check< T >& check1, const Check< T >& check2 ):
                _check1( check1.clone() ),
                _check2( check2.clone() )
      {
      }
      
      bool check( const T& value ) const
      {
	return _check1->check( value ) && _check2->check( value );
      }
      
    private:
      typename CheckPtr< T >::Type _check1,
                                   _check2;
    };
    
    /*! Chechs that at least one containing check is true */
    template< class T >
    class OrCheck : public check::Clonable< OrCheck, T >
    {
    public:
      /*! Constructs check from operand check objects */
      OrCheck( const Check< T >& check1, const Check< T >& check2 ):
               _check1( check1.clone() ),
               _check2( check2.clone() )
      {
      }
      
      bool check( const T& value ) const
      {
	return _check1->check( value ) || _check2->check( value );
      }
      
    private:
      typename CheckPtr< T >::Type _check1,
                                   _check2;
    };
    
    /*! Check that containing check is false */
    template< class T >
    class NotCheck : public check::Clonable< NotCheck, T >
    {
    public:
      /*! Constructs check from negating check object */
      NotCheck( const Check< T >& check ):
                _check( check.clone() )
      {
      }
      
      bool check( const T& value ) const
      {
	return !_check->check( value );
      }
      
    private:
      typename CheckPtr< T >::Type _check;
    };
    
    /*! Checks that experssion is equal to value */
    template< class T >
    class EqualCheck : public check::Clonable< EqualCheck, T >
    {
    public:
      /*! Constructs check */
      EqualCheck( const T& value ):
                  _value( value )
      {
      }
      
      bool check( const T& value ) const
      {
	return value == _value;
      }
      
    private:
      T _value;
    };
    
    /*! Checks that experssion is not equal to value */
    template< class T >
    class NotEqualCheck : public check::Clonable< NotEqualCheck, T >
    {
    public:
      /*! Constructs check */
      NotEqualCheck( const T& value ):
                     _value( value )
      {
      }
      
      bool check( const T& value ) const
      {
	return value != _value;
      }
      
    private:
      T _value;
    };
    
    /*! Checks that experssion is less than value */
    template< class T >
    class LessCheck : public check::Clonable< LessCheck, T >
    {
    public:
      /*! Constructs check */
      LessCheck( const T& value ):
                 _value( value )
      {
      }
      
      bool check( const T& value ) const
      {
	return value < _value;
      }
      
    private:
      T _value;
    };
    
    /*! Checks that experssion is more than value */
    template< class T >
    class MoreCheck : public check::Clonable< MoreCheck, T >
    {
    public:
      /*! Constructs check */
      MoreCheck( const T& value ):
                 _value( value )
      {
      }
      
      bool check( const T& value ) const
      {
	return value > _value;
      }
      
    private:
      T _value;
    }; 
    
    /*! Checks that experssion is less or equal to value */
    template< class T >
    class LessEqualCheck : public check::Clonable< LessEqualCheck, T >
    {
    public:
      /*! Constructs check */
      LessEqualCheck( const T& value ):
                      _value( value )
      {
      }
      
      bool check( const T& value ) const
      {
	return value <= _value;
      }
      
    private:
      T _value;
    }; 
    
    /*! Checks that experssion is more or equal to value */
    template< class T >
    class MoreEqualCheck : public check::Clonable< MoreEqualCheck, T >
    {
    public:
      /*! Constructs check */
      MoreEqualCheck( const T& value ):
                      _value( value )
      {
      }
      
      bool check( const T& value ) const
      {
	return value >= _value;
      }
      
    private:
      T _value;
    }; 
  }
}
#endif
