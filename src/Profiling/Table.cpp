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

#include <algorithm>
#include <boost/foreach.hpp>
#include "Table.hpp"

using std::max;
using std::string;
using std::vector;
using namespace burning::profiling;

Table::Table(): _table(),
                _columns( 0 )
{
}

size_t Table::columns() const
{
  return _columns.size();
}

size_t Table::rows() const
{
  return _table.size();     
}

Table::RowProxy Table::operator[]( size_t index )
{
  return row( index );
}

Table::RowProxy Table::row( size_t index )
{
  if( _table.size() <= index )
    _table.resize( index + 1 );
  
  return RowProxy( _table[ index ], _columns, index );
}

Table::ColumnProxy Table::column( const string& name )
{
  size_t index( _columns.size() );
  
  for( size_t i=0; i<_columns.size(); i++ )
    if( _columns[ i ].name == name )
    {
      index = i;
      break;
    }
    
  if( index == _columns.size() )
    _columns.push_back( Column( name, 0 ) );
  
  return column( index );
}

Table::ColumnProxy Table::column( size_t index )
{
  if( index >= _columns.size() )
    _columns.resize( index + 1 );
  
  return ColumnProxy( _table, _columns[ index ], index );
}

Table::ColumnProxy Table::newColumn()
{
  return column( columns() );
}

Table::RowProxy Table::newRow()
{
  return row( rows() );
}

void Table::print( std::ostream& ostream )
{
  doPrint< TextPrinter >( ostream );
}

void Table::printHtml( std::ostream& ostream )
{
  doPrint< HtmlPrinter >( ostream );
}

template< class Printer >
void Table::doPrint( std::ostream& ostream )
{
  Printer printer( ostream );
  
  printer.begin();
  
  vector< bool > equalMeasured( _columns.size() );
  for( size_t i=0; i<_columns.size(); i++ )
    equalMeasured[ i ] = isEqualMeasured( column( i ) );
    
  if( haveHeader() )
    printHeader< Printer >( ostream, equalMeasured );
  
  for( size_t i=0; i<rows(); i++ )
  {
    printer.beginRow();
    
    RowProxy currentRow( row( i ) );
    for( size_t j=0; j<currentRow.columns(); j++ )
    {
      string measure( "" );
      if( !equalMeasured[ j ] )
	measure = currentRow[ j ].measure();
      
      printer.beginValue();
      printValue( currentRow[ j ].value(), measure, ostream );
      printer.endValue();
    }
    
    printer.endRow();
  }
  
  printer.end();
}

bool Table::isEqualMeasured( ColumnProxy col )
{
  if( col.name() == "" )
    return false;
  
  if( col.rows() == 0 )
    return false;
  
  for( size_t i=0; i<col.rows(); i++ )
    if( col[ i ].measure() != col[ 0 ].measure() )
      return false;
    
  return true;
}

bool Table::haveHeader()
{
  for( size_t i=0; i< columns(); i++ )
    if( _columns[ i ].name != "" )
      return true;
  return false;
}

template< class Printer >
void Table::printHeader( std::ostream& ostream, const vector< bool >& equalMeasured )
{
  Printer printer( ostream );
  
  printer.beginHeader();
  for( size_t i=0; i< columns(); i++ )
  {
    string measure;
    if( equalMeasured[ i ] )
      measure = column( i )[ 0 ].measure();
    else
      measure = "";
    
    printer.beginValue();
    printValue( _columns[ i ].name, measure, ostream );
    printer.endValue();
  }
  printer.endHeader();
}

template< class T >
void Table::printValue( const T& value, const string& measure, std::ostream& ostream )
{
  ostream << value;
 
  if( measure != "" )
    ostream << " (" << measure << ')';
}

/*
 *  TextPrinter
 */

class Table::TextPrinter
{
public:
  TextPrinter( std::ostream& ostream ) : _ostream( ostream )
  {
  }
  
  void begin();
  void end();
  
  void beginRow();
  void endRow();
  
  void beginValue();
  void endValue();
  
  void beginHeader();
  void endHeader();
private:
  
  std::ostream& _ostream;
};

void Table::TextPrinter::begin()
{
}

void Table::TextPrinter::end()
{
}

void Table::TextPrinter::beginRow()
{
}

void Table::TextPrinter::endRow()
{
  _ostream << '\n';
}

void Table::TextPrinter::beginValue()
{
}

void Table::TextPrinter::endValue()
{
  _ostream << '\t';
}

void Table::TextPrinter::beginHeader()
{
}

void Table::TextPrinter::endHeader()
{
  _ostream << '\n';
}

/*
 *  HtmlPrinter
 */

class Table::HtmlPrinter
{
public:
  HtmlPrinter( std::ostream& ostream ): _ostream( ostream ),
                                        _inHeader( false )
  {
  }
  
  void begin();
  void end();
  
  void beginRow();
  void endRow();
  
  void beginValue();
  void endValue();
  
  void beginHeader();
  void endHeader();
private:
  
  std::ostream& _ostream;
  bool _inHeader;
};

void Table::HtmlPrinter::begin()
{
  _ostream << "<table border=1>\n";
}

void Table::HtmlPrinter::end()
{
  _ostream << "</table>\n";
}

void Table::HtmlPrinter::beginRow()
{
  _ostream << "<tr>";
}

void Table::HtmlPrinter::endRow()
{
  _ostream << "</tr>\n";
}

void Table::HtmlPrinter::beginValue()
{
  if( _inHeader )
    _ostream << "<th>";
  else
    _ostream << "<td>";
}

void Table::HtmlPrinter::endValue()
{
  if( _inHeader )
    _ostream << "</th>";
  else
    _ostream << "</td>";
}

void Table::HtmlPrinter::beginHeader()
{
  _ostream << "<tr>";
  _inHeader = true;
}

void Table::HtmlPrinter::endHeader()
{
  _ostream << "</tr>\n";
  _inHeader = false;
}

/*
 * RowProxy
 */

Table::RowProxy::RowProxy( Row& row, std::vector< Column >& columns, size_t index ) : 
                           _row( row ),
                           _columns( columns ),
                           _index( index )
{
}

Value& Table::RowProxy::operator[]( const string& name )
{
  size_t index( _columns.size() );
  for( size_t i=0; i<_columns.size(); i++ )
    if( _columns[ i ].name == name )
    {
      index = i;
      break;
    }
    
  if( index == _columns.size() )
    _columns.push_back( Column( name, 0 ) );
  
  return operator[]( index );
}

Value& Table::RowProxy::operator[]( size_t index )
{
  if( _row.size() <= index )
  {
    _row.resize( index + 1 );
    size_t newColumnsSize = max( _columns.size(), index + 1 );
    _columns.resize( newColumnsSize );
    
    _columns[ index ].rows = max( _columns[ index ].rows, _index + 1 );
  }
  
  return _row[ index ];
}

void Table::RowProxy::pushBack( const Value& value )
{
  operator[]( columns() ) = value;
}

size_t Table::RowProxy::columns() const
{
  return _row.size();
}

/*
 * ColumnProxy
 */

Table::ColumnProxy::ColumnProxy( std::vector< Table::Row >& table, Column& column, size_t index ):
                                  _table( table ),
                                  _column( column ),
                                  _index( index )
{
}

size_t Table::ColumnProxy::rows() const
{
  return _column.rows;
}

Value& Table::ColumnProxy::operator[]( size_t index )
{
  if( _table.size() <= index )
    _table.resize( index + 1 );
  
  if( _table[ index ].size() <= _index )
    _table[ index ].resize( _index + 1 );
  
  if( _column.rows <= index )
    _column.rows = index + 1;
  
  return _table[ index ][ _index ];
}

void Table::ColumnProxy::pushBack( const Value& value )
{
  operator[]( _column.rows ) = value;
}

string& Table::ColumnProxy::name()
{
  return _column.name;
}
