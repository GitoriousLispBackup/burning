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

#ifndef BURNING_COMMANDLINE_CONTAINERTRAITS_HPP
#define BURNING_COMMANDLINE_CONTAINERTRAITS_HPP
#include <vector>
#include <list>
#include <set>

namespace burning
{
  namespace commandLine
  {
    
    /*! A type wrap for bool */
    template< bool val >
    struct BoolType
    {
      /*! A value containing in type */
      static const bool value = val;
    };
    
    typedef BoolType< false > FalseType;
    typedef BoolType< true > TrueType;
    
    /*! An information about type T */
    template< class T >
    struct Type
    {
      /*! Checks that type T is container.
       *  Equal to TrueType if yes
       *  FalseType otherwise
       */
      typedef FalseType IsContainer;
      
      /*! A contained type for T.
       *  Is a void type if T is not container
       */
      typedef void ContainedType;
    };
   
    /*! An implementation of class information for vector */
    template< class T >
    struct Type< std::vector< T > >
    {
      /*! An IsContainer type for vector.
       *  It is a true type.
       */
      typedef TrueType IsContainer;
      /*! A contained type for vector */
      typedef T ContainedType;
    };
    
    /*! An implementation of class information for list */
    template< class T >
    struct Type< std::list< T > >
    {
      /*! An IsContainer type for list.
       *  It is a true type.
       */
      typedef TrueType IsContainer;
      /*! A contained type for list */
      typedef T ContainedType;
    };
    
    /*! An implementation of class information for set */
    template< class T >
    struct Type< std::set< T > >
    {
      /*! An IsContainer type for set.
       *  It is a true type.
       */
      typedef TrueType IsContainer;
      
      /*! A contained type for set */
      typedef T ContainedType;
    }; 
  }
}
#endif
