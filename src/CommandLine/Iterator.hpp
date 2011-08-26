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

#ifndef BURNING_COMMANDLINE_ITERATOR_HPP
#define BURNING_COMMANDLINE_ITERATOR_HPP
#include <string>

namespace burning
{
  namespace commandLine
  {
    /*! An iterator for command line */
    class Iterator
    {
    public:
      /*! Construct iterator from command line and position in it. */
      Iterator( int argc, const char* argv[], int position = 0 );
      
      /*! Checks that iterator at end of command line */
      bool atEnd() const;
      /*! Checks that iterator at command line's argument */
      bool atArgument() const;
      
      /*! Increments iterator */
      void operator++();
      /*! Increments iterator */
      void operator++( int );
      
      /*! Decrements iterator */
      void operator--();
      /*! Decrements iterator */
      void operator--( int );
      
      /*! A value of current argument */
      std::string operator*() const;
      
    private:
      
      int _argc;
      const char** _argv;
      
      int _pos;
    };
  }
}
#endif
