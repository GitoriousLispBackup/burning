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

#include <stdlib.h>
#include <glog/logging.h>
#include "Iterator.hpp"

using std::string;
using namespace burning::commandLine;

Iterator::Iterator( int argc, const char* argv[], int position ):
                    _argc( argc ),
                    _argv( argv ),
                    _pos( position )
{
}

bool Iterator::atArgument() const
{
  string curr( operator*() );
  return curr.find( '-' ) == 0;
}

bool Iterator::atEnd() const
{
  return _pos >= _argc;
}

void Iterator::operator++()
{
  ++_pos;
}

void Iterator::operator++( int )
{
  _pos++;
}

void Iterator::operator--()
{
  --_pos;
}

void Iterator::operator--( int )
{
  --_pos;
}

string Iterator::operator*() const
{
  if( _pos < 0 )
  {
    LOG( ERROR ) << "Tried to read at negative position.";
    exit( EXIT_FAILURE );
  }
  if( atEnd() )
  {
    LOG( ERROR ) << "Tried to read command line argument " << _pos + 1 << ", when command line has " << _argc << " arguments.";
    exit( EXIT_FAILURE );
  }
  return _argv[ _pos ];
}
