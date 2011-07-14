project( burning )
cmake_minimum_required(VERSION 2.6)

set( CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )
set( TEST_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/gtest/main.cpp )

include( filesystem )
include( configure_project )

set( BASE_PATH ${CMAKE_CURRENT_SOURCE_DIR} )
set( SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src )
# set( EXAMPLES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/examples )

set( LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set( BINARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin )
# set( EXAMPLES_BINARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/examples )

set( DOC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/doc/ )

set( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install )

set( LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib )
set( BINARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin )
set( INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include )
set( DOC_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/doc )

enable_testing()
create_directory( tests )

option( INSTALL_EXAMPLES "Install examples." ON )
option( INSTALL_DOCUMENTATION "Install documentation." ON )

# add_subdirectory( src/Profiling Profiling )
# add_subdirectory( src/Core Core )
add_subdirectory( src/Xml Xml )
# add_subdirectory( src/Graphics Graphics )
# add_subdirectory( src/Compute Compute )
# add_subdirectory( src/Scenegraph Scenegraph )
# add_subdirectory( src/Convert Convert )
# add_subdirectory( src/QtGui QtGui )
# add_subdirectory( src/Repository Repository )
# add_subdirectory( src/Rendering Rendering )
# add_subdirectory( src/CommandLine CommandLine )

# add_subdirectory( utils utils )

# find_package( Doxygen )
# if( DOXYGEN_FOUND )

#   add_custom_target( doc
#                      ${DOXYGEN_EXECUTABLE} ${DOC_PATH}/Doxyfile
#                      WORKING_DIRECTORY ${DOC_PATH}
#                    )
#   if( INSTALL_DOCUMENTATION )
#     install( DIRECTORY ${DOC_PATH}/html ${DOC_PATH}/latex DESTINATION ${DOC_INSTALL_PATH} )
#   endif()
# endif()