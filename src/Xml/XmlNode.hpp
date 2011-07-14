#ifndef RTRT_XML_XMLNODE_HPP
#define RTRT_XML_XMLNODE_HPP

#include <string>
#include <istream>
#include <ostream>
#include <map>
#include <boost/tr1/memory.hpp>
#include "XmlAttributeProxy.hpp"
#include "XmlNodesProxy.hpp"

namespace rtrt
{
  /*! A node in XML hierarchy */
  class XmlNode
  {
  public:
    /*! Creates empty node with specified name */
    static XmlNodePtr create( const std::string& name = "" );
    
    /*! Constructs node with specified name */
    explicit XmlNode( const std::string& name = "" );
    
    /*! Node's name */
    std::string& name();
    /*! Node's name */
    const std::string& name() const;
    
    /*! Writes node in XML format to specified ostream */
    void write( std::ostream& ostream, int indenation = 0 ) const;
    /*! Parses node from specified istream */
    static XmlNodePtr parse( std::istream& istream );
    
    /*! Node's attribute with specified name */
    XmlAttributeProxy attr( const std::string& name );
    
    /*! Node's childs */
    InsertableXmlNodesProxy childs();
    /*! Node's childs */
    const XmlNodesProxy childs() const;
    
    /*! Node's childs with specified name */
    XmlNodesProxy childs( const std::string& name );
    /*! Node's childs with specified name */
    const XmlNodesProxy childs( const std::string& name ) const;
    
    /*! Parent node */
    XmlNode* parent();
    
  private:
    
    friend class InsertableXmlNodesProxy;
    void addChild( XmlNodePtr child );
    
    XmlNode( const XmlNode& );
    XmlNode& operator=( const XmlNode& );
    
    static std::string makeIndentation( int indentation );
    std::string makeOpeningTag( int indentation ) const;
    std::string makeClosingTag( int indentation ) const;
    
    std::string _name; 
    
    AttributeMap _attributes;
    NodeSet _childs;
    XmlNode* _parent;
  };
}
#endif