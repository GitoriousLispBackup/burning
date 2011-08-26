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
#include <boost/foreach.hpp>
#include <glog/logging.h>
#include "CommandLine.hpp"

using std::string;
using namespace burning;
using namespace burning::commandLine;

CommandLine::CommandLine( const std::string& programName ):
                          Action( programName )
{
}

void CommandLine::parse( int argc, const char* argv[] )
{
  Iterator iter( argc, argv );
  iter++;
  
  for(; !iter.atEnd();)
  {
    string arg( *iter );
    iter++;
    
    if( !parsePosition( arg, iter ) )
    {
      LOG( ERROR ) << arg << " is not a arguments name.";
      exit( EXIT_FAILURE );
    }
  }
}
