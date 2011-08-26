include(FindPackageHandleStandardArgs)
include(ConfigurePackage)

find_prerequests( Xml "" GLOG )

find_path( Xml_PRIMARY_INCLUDE_DIR  "Xml/Node.hpp" ${SOURCE_PATH} )  
set( Xml_LIBRARIES Xml )

ConfigurePackage( Xml )