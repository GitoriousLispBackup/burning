#ifndef RTRT_XML_NODE_HPP
#define RTRT_XML_NODE_HPP

#include <string>
#include <istream>
#include <ostream>
#include <map>
#include <boost/tr1/memory.hpp>
#include "AttributeProxy.hpp"
#include "NodesProxy.hpp"

namespace bng
{
  namespace xml
  {
    /*! A node in XML hierarchy */
    class Node
    {
    public:
      /*! Creates empty node with specified name */
      static NodePtr create( const std::string& name = "" );
    
      /*! Constructs node with specified name */
      explicit Node( const std::string& name = "" );
    
      /*! Node's name */
      std::string& name();
      /*! Node's name */
      const std::string& name() const;
    
      /*! Writes node in XML format to specified ostream */
      void write( std::ostream& ostream, int indenation = 0 ) const;
      /*! Parses node from specified istream */
      static NodePtr parse( std::istream& istream );
    
      /*! Node's attribute with specified name */
      AttributeProxy attr( const std::string& name );
    
      /*! Node's childs */
      InsertableNodesProxy childs();
      /*! Node's childs */
      const NodesProxy childs() const;
    
      /*! Node's childs with specified name */
      NodesProxy childs( const std::string& name );
      /*! Node's childs with specified name */
      const NodesProxy childs( const std::string& name ) const;
    
      /*! Parent node */
      Node* parent();
    
    private:
    
      friend class InsertableNodesProxy;
      void addChild( NodePtr child );
    
      Node( const Node& );
      Node& operator=( const Node& );
    
      static std::string makeIndentation( int indentation );
      std::string makeOpeningTag( int indentation ) const;
      std::string makeClosingTag( int indentation ) const;
    
      std::string _name; 
    
      AttributeMap _attributes;
      NodeSet _childs;
      Node* _parent;
    };
  }
}
#endif