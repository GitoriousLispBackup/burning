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

#ifndef BURNING_PROFILING_PROFILING_HPP
#define BURNING_PROFILING_PROFILING_HPP

#ifdef USE_PROFILING

#define BEGIN_PROFILING burning::profiling::beginProfiling()
#define END_PROFILING burning::profiling::endProfiling()

#define PROFILING_BEGIN_LOOP( name ){ burning::profiling::beginLoop( name ); }
#define PROFILING_END_LOOP { burning::profiling::endLoop(); }
#define PROFILING_BEGIN_ITERATION( name ){ burning::profiling::beginIteration( name ); }
#define PROFILING_END_ITERATION { burning::profiling::endIteration( burning::profiling::milliseconds ); }
#define PROFILING_BEGIN_PHASE( name ){ burning::profiling::beginPhase( name ); }
#define PROFILING_END_PHASE { burning::profiling::endPhase( burning::profiling::milliseconds ); }
#define PROFILING_END_PHASE_MICROSECONDS {  burning::profiling::endPhase( burning::profiling::microseconds ); }
#define PROFILING_END_PHASE_SECONDS {  burning::profiling::endPhase( burning::profiling::seconds ); }
#define PROFILING_ADD_VALUE( name, value ) { burning::profiling::addValue( name, value ); }
#define PROFILING_ADD_VALUE_MEASURED( name, value, measure ) { burning::profiling::addValue( name, value, measure ); }

#else

#define BEGIN_PROFILING
#define END_PROFILING

#define PROFILING_BEGIN_LOOP( name )
#define PROFILING_END_LOOP( name )
#define PROFILING_BEGIN_ITERATION( name )
#define PROFILING_END_ITERATION( name )
#define PROFILING_BEGIN_PHASE( name ) 
#define PROFILING_END_PHASE 
#define PROFILING_END_PHASE_MICROSECONDS
#define PROFILING_END_PHASE_SECONDS
#define PROFILING_ADD_VALUE( name, value )
#define PROFILING_ADD_VALUE_MEASURED( name, value, measure )

#endif

#include <string>
#include <Xml/Attribute.hpp>

namespace burning
{
  namespace profiling
  {
    enum TimeMeasure
    {
      seconds = 1,
      milliseconds = 1000,
      microseconds = 1000000,
      nanoseconds = 1000000000
    };
    
    void beginPhase( const std::string& name );
    void endPhase( TimeMeasure measure );
    
    void beginLoop( const std::string& name );
    void endLoop();
    
    void beginIteration( const burning::xml::Attribute::ValueType& name );
    void endIteration( TimeMeasure measure );
    
    void addValue( const std::string& name, const burning::xml::Attribute::ValueType& value );
    void addValue( const std::string& name, const burning::xml::Attribute::ValueType& value, const std::string& measure );
    
    void beginProfiling();
    void endProfiling();
  }
}

#endif
