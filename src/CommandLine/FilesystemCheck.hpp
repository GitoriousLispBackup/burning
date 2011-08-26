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

#ifndef BURNING_COMMANDLINE_FILESYSTEMCHECK_HPP
#define BURNING_COMMANDLINE_FILESYSTEMCHECK_HPP
#include <string>
#include <glog/logging.h>
#include <boost/filesystem.hpp>
#include "Check.hpp"

namespace burning
{
  namespace commandLine
  {
    /*! Checks that value is name of existing file */
    template< class T >
    class ExistingFileCheckImpl : public check::Clonable< ExistingFileCheckImpl, T >
    {
    public:
      bool check( const T& value ) const
      {
	if( !boost::filesystem::exists( value ) )
	{
	  LOG( ERROR ) << "File " << value << " does not exists.";
	  return false;
	}
	if( boost::filesystem::is_directory( value ) )
	{
	  LOG( ERROR ) << value << " is a directory.";
	  return false;
	}
	
	return true;
      }
    }; 
    typedef ExistingFileCheckImpl< std::string > ExistingFileCheck;
    
    /*! Checks that value is name of existing directory */
    template< class T >
    class ExistingDirectoryCheckImpl : public check::Clonable< ExistingDirectoryCheckImpl, T >
    {
    public:
      bool check( const T& value ) const
      {
	if( !boost::filesystem::exists( value ) )
	{
	  LOG( ERROR ) << "Directory " << value << " does not exists.";
	  return false;
	}
	if( !boost::filesystem::is_directory( value ) )
	{
	  LOG( ERROR ) << value << " is a file.";
	  return false;
	}
	
	return true;
      }
    }; 
    typedef ExistingDirectoryCheckImpl< std::string > ExistingDirectoryCheck;
    
    /*! Checks that value is name of correct file */
    template< class T >
    class CorrectFileCheckImpl : public check::Clonable< CorrectFileCheckImpl, T >
    {
    public:
      bool check( const T& value ) const
      {
	boost::filesystem::path path( value );
	
	if( !boost::filesystem::exists( path.parent_path() ) && path.parent_path() != "" )
	{
	  LOG( ERROR ) << "Directory " << path.parent_path() << " does not exists.";
	  return false;
	}
	if( boost::filesystem::is_directory( value ) )
	{
	  LOG( ERROR ) << value << " is a directory.";
	  return false;
	}
	
	return true;
      }
    }; 
    typedef CorrectFileCheckImpl< std::string > CorrectFileCheck;
  }
}
#endif

