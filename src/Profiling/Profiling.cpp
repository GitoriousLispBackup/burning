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

#include "Profile.hpp"
#include "Profiling.hpp"

using namespace burning;

static bool useProfiling = false;

void profiling::beginPhase( const std::string& name )
{
  if( useProfiling )
    Profile::global().beginPhase( name );
}

void profiling::endPhase( TimeMeasure measure )
{
  if( useProfiling )
    Profile::global().endPhase( measure );
}
    
void profiling::addValue( const std::string& name, const xml::Attribute::ValueType& value )
{
  if( useProfiling )
    Profile::global().addValue( name, value );
}

void profiling::addValue( const std::string& name, const xml::Attribute::ValueType& value, const std::string& measure )
{
  if( useProfiling )
    Profile::global().addValue( name, profiling::Value( value, measure ) );
}

void profiling::beginLoop( const std::string& name )
{
  if( useProfiling )
    Profile::global().beginLoop( name );
}

void profiling::endLoop()
{
  if( useProfiling )
    Profile::global().endLoop();
}

void profiling::beginIteration( const xml::Attribute::ValueType& name )
{
  if( useProfiling )
    Profile::global().beginIteration( name );
}

void profiling::endIteration( TimeMeasure measure )
{
  if( useProfiling )
    Profile::global().endIteration( measure );
}

void profiling::beginProfiling()
{
  useProfiling = true;
}

void profiling::endProfiling()
{
  useProfiling = false;
}
