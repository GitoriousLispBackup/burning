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

#ifndef BURNING_PROFILING_PHASE_HPP
#define BURNING_PROFILING_PHASE_HPP

#include <iostream>
#include <map>
#include <glog/logging.h>
#include <boost/lexical_cast.hpp>
#include <boost/tr1/memory.hpp>
#include <Xml/Node.hpp>
#include "Value.hpp"
#include "Profiling.hpp"

namespace burning
{
  namespace profiling
  {
    class Table;
    class Phase;
    typedef std::tr1::shared_ptr< Phase > PhasePtr;
    
    /*! A phase of program execution */
    class Phase
    {
    public:
      Phase();
      /*! Constructs new phase as a copy of existing one */
      Phase( const Phase& phase );
      
      /*! Copies phase value from an existing one */
      void operator=( const Phase& phase );
      
      /*! A collection of iteration's names */
      typedef std::vector< xml::Attribute > IterationVector;
      
      /*! Names of iterations for current phase */
      IterationVector& iterations()
      {
	return _iterations;
      }
      /*! Names of iterations for current phase */
      const IterationVector& iterations() const
      {
	return _iterations;
      }
      
      /*! A collection of subphases.
       *  First value is a name of subphase
       *  Second is a collection of phases for each iteration 
       */
      typedef std::map< std::string, std::vector< PhasePtr > > PhaseMap;
      /*! Collection of subphases for current phase */
      const PhaseMap& phases() const;
      
      /*! Adds new phase for current iteration */
      void addPhase( const std::string& name, const profiling::PhasePtr& phase );
      
      /*! A collection of values
       * First value is a name of value
       * Second is a collection of values for each iteration\
       */
      typedef std::map< std::string, std::vector< profiling::Value > > ValueMap;
      
      /*! Collection of values for phase */
      const ValueMap& values() const;
      
      /*! Adds new value to current iteration */
      void addValue( const std::string& name, const profiling::Value& value );
      
      /*! Begins new iteration */
      void beginIteration( const xml::Attribute::ValueType& name );
      /*! End current iteration */
      void endIteration( TimeMeasure timeMeasure = milliseconds );
      
      /*! Checks that there is not iterations currently in progress */
      bool finished()
      {
	return !_began;
      }
      
      /*! Checks that phase is loop */
      bool isLoop();
      
      /*! Converts phase's information to xml format. */
      xml::NodePtr toXml();
      /*! Restores information about phase from xml data */
      static PhasePtr fromXml( xml::Node& node );
      
      /*! Saves phases' information in table */
      void toTable( Table* table );
      
    private:
      xml::NodePtr iterationToXml( size_t index );
      
      void setValueFromXml( const xml::NodePtr& value );
      void iterationFromXml( xml::Node& node );
      void subphaseFromXml( xml::Node& node );
      
      void iterationsToTable( Table* table );
      
      ValueMap _values;
      PhaseMap _phases;
      IterationVector _iterations;
      
      timespec _beginTime;
      bool _began;
    };
  }
}
#endif
