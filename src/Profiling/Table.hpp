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

#ifndef BURNING_PROFILING_TABLE_HPP
#define BURNING_PROFILING_TABLE_HPP
#include <ostream>
#include <string>
#include <vector>
#include "Value.hpp"

namespace burning
{
  namespace profiling
  {
    /*!
     * Class representing table data.
     */
    class Table
    {
    public:
      Table();
      
      /*! Rows in table */
      size_t rows() const;
      /*! Columns in table */
      size_t columns() const;
      
      /*! Type for row */
      typedef std::vector< Value > Row;
      
      class RowProxy;
      /*! Gets row by index */
      RowProxy operator[]( size_t index );
      /*! Gets row by index */
      RowProxy row( size_t index );
      /*! Adds row to table */
      RowProxy newRow();
      
      class ColumnProxy;
      /*! Gets column by index */
      ColumnProxy column( size_t index );
      /*! Gets column by name */
      ColumnProxy column( const std::string& name );
      /*! Adds column to table */
      ColumnProxy newColumn();
      
      /*! Print table as a simple plain table */
      void print( std::ostream& ostream );
      /*! Print table in html format */
      void printHtml( std::ostream& ostream );
      
    private:
      struct Column 
      {
	Column() : name(),
	           rows()
	{
	}
	
	Column( const std::string& nm, size_t rws ) : name( nm ),
	                                              rows( rws )
	{
	}
	
	std::string name;
	size_t rows;
      };      
      
      template< class Printer >
      void doPrint( std::ostream& ostream );
      
      class TextPrinter;
      class HtmlPrinter;
      
      static bool isEqualMeasured( ColumnProxy col );
      
      template< class T >
      void printValue( const T& value, const std::string& measure, std::ostream& ostream );
      
      bool haveHeader();
      template< class Printer >
      void printHeader( std::ostream& ostream, const std::vector< bool >& equalMeasured );
      
      std::vector< Row > _table;

      
      std::vector< Column > _columns;
    };
    
    /*! Representing row in table */
    class Table::RowProxy
    {      
    public:
      /*! Constructs proxy */
      RowProxy( Row& row, std::vector< Column >& columns, size_t index ); 
      /*! Constructs proxy */
      RowProxy( const RowProxy& proxy );
      
      /*! Sets proxy data from other proxy */
      void operator=( const RowProxy& proxy );
	
      /*! Gets element from row by columns index */
      Value& operator[]( size_t index );
      /*! Gets element from row by columns name */
      Value& operator[]( const std::string& name );
      /*! Adds new value to row */
      void pushBack( const Value& value );
	
      /*! Columns in row */
      size_t columns() const;
    private:
      Row& _row;
      std::vector< Column >& _columns;
      size_t _index;
    };
    
    /*! Representing column in table */
    class Table::ColumnProxy
    {
    public:
      /*! Constructs proxy */
      ColumnProxy( std::vector< Row >& table, Column& column, size_t index );
      /*! Constructs proxy */
      ColumnProxy( const ColumnProxy& proxy );
      
      /*! Sets proxy data from other proxy */
      void operator=( const ColumnProxy& proxy );
      
      /*! Gets value using element's index */
      Value& operator[]( size_t index );
      /*! Adds new value to column */
      void pushBack( const Value& value );
      
      /*! Rows in column */
      size_t rows() const;
      /*! Column's name */
      std::string& name();
      
    private:
      std::vector< Row >& _table;
      Column& _column;
      size_t _index;
    };
  }
}
#endif
