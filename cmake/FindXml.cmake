include(FindPackageHandleStandardArgs)
include(ConfigurePackage)

find_prerequests( Xml "" GLOG )

find_path( Xml_PRIMARY_INCLUDE_DIR  "Xml/XmlNode.hpp" ${SOURCE_PATH} )  
set( Xml_LIBRARIES Xml )

ConfigurePackage( Xml )