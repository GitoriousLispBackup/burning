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

#ifndef BURNING_PROFILING_PROFILE_HPP
#define BURNING_PROFILING_PROFILE_HPP

#include <string>
#include <stack>
#include <vector>
#include "Phase.hpp"

namespace burning
{
  class Profile;
  typedef std::tr1::shared_ptr< Profile > ProfilePtr;
  
  //! Object stores information about work of program
  class Profile
  {
  public:
    Profile();
    
    /*! Adds new attribute to profile */
    void addValue( const std::string& name, const profiling::Value& value );
    
    /*! Begins recording of a loop */
    void beginLoop( const std::string& name );
    /*! Ends recording of a loop */
    void endLoop();
    
    //! Begins new phase of timing
    void beginPhase( const std::string& name );
    //! Ends current phase of timing
    void endPhase( profiling::TimeMeasure measure = profiling::milliseconds );
    
    /*! Begins recording of iteration */
    void beginIteration( const xml::Attribute::ValueType& name );
    /*! Ends recording of iteration */
    void endIteration( profiling::TimeMeasure measure = profiling::milliseconds );
    
    //! Global object used for storing information
    static Profile& global();
    
    /*! A phase object representing whole measured program */
    const profiling::Phase& rootPhase()
    {
      return *_rootPhase;
    }
    
    /*! Converts profiling result to xml */
    xml::NodePtr toXml();
    /*! Restored profiling result from xml */
    static ProfilePtr fromXml( xml::Node& node );
    
    /*! Prints profiling result as a table */
    void print( std::ostream& ostream = std::cout );
    /*! Prints profiling result in html format */
    void printHtml( std::ostream& ostream = std::cout );
    
  private:
    static Profile& _global;
    bool preparePrint( profiling::Table* table, profiling::PhasePtr& root, bool printed = false );
    
    profiling::PhasePtr _rootPhase;
    std::stack< profiling::Phase* > _current;
  };
  
}

#endif
