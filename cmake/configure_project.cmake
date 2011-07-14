#
# USE_RELEASE Builds program in release mode
# USE_DEBUG Builds porgram with debug information
#
macro( configure_compiler ${USE_RELEASE} ${USE_DEBUG} )
if( CMAKE_COMPILER_IS_GNUCXX )
  add_definitions( -Wall -Wwrite-strings -Wno-long-long -Werror )
endif()

if( ${USE_RELEASE} )
  if( ${USE_DEBUG} )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELWITHDEBINFO}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" )
  else()
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}" )
  endif()
else()
 set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" )
 set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}" )
endif()
endmacro()

#
# Sets option for project and configures compiler
#
macro( configure_project )
  option( ${PROJECT_NAME}_USE_DEBUG "Compile with debug info." ON )
  option( ${PROJECT_NAME}_USE_RELEASE "Compile in release mode." ON )
  option( ${PROJECT_NAME}_BUILD_EXAMPLES "Build with examples." ON )

  set( USE_DEBUG ${PROJECT_NAME}_USE_DEBUG )
  set( USE_RELEASE ${PROJECT_NAME}_USE_RELEASE )
  
  configure_compiler( USE_RELEASE USE_DEBUG )
endmacro()

#
# Configures example using settings of associatied library.
#
macro( configure_example )
  set( USE_DEBUG ${LIBRARY_NAME}_USE_DEBUG )
  set( USE_RELEASE ${LIBRARY_NAME}_USE_RELEASE )

  configure_compiler( USE_RELEASE USE_DEBUG )
endmacro()

#  INPUT VARIABLES:
#
#  NAME - name of current project
#  OPTIONS - FIND OPTIONS( REQUIRED AND QUIET - see CMake find_package documentation.
#  ${NAME}_BOOST_COMPONENTS - used additional boost components
#
#  OUTPUT_VARIABLES
#
#  ${NAME}_PREREQUESTS_INCLUDE_DIR - include directories for prerequests of current project
#  ${NAME}_PREREQUESTS_LIBRARIES - libraries for prerequests of current project.
#

macro( find_prerequests NAME OPTIONS ${ARGC} )
  set( ${NAME}_PREREQUESTS_FOUND TRUE )

  foreach( PACKAGE ${ARGN} )

    if( ${PACKAGE} STREQUAL Boost )
       find_package( Boost ${OPTIONS} COMPONENTS ${${NAME}_BOOST_COMPONENTS} )
    else()
      find_package( ${PACKAGE} ${OPTIONS} )
    endif()

    string( TOUPPER ${PACKAGE} UPPER_PACKAGE )

    if( NOT ${PACKAGE}_FOUND AND NOT ${UPPER_PACKAGE}_FOUND )
      set( ${NAME}_PREREQUESTS_FOUND FALSE )
    endif()
 
    list( APPEND ${PACKAGE}_INCLUDE_DIR ${${UPPER_PACKAGE}_INCLUDE_DIR} )
    list( APPEND ${PACKAGE}_LIBRARIES ${${UPPER_PACKAGE}_LIBRARIES} )

    list( APPEND ${NAME}_PREREQUESTS_INCLUDE_DIR ${${PACKAGE}_INCLUDE_DIR} )
    list( APPEND ${NAME}_PREREQUESTS_LIBRARIES ${${PACKAGE}_LIBRARIES} )
 
  endforeach( PACKAGE )

  list( REMOVE_DUPLICATES ${NAME}_PREREQUESTS_INCLUDE_DIR )
  if( DEFINED ${NAME}_PREREQUESTS_LIBRARIES )
    list( REMOVE_DUPLICATES ${NAME}_PREREQUESTS_LIBRARIES )
  endif()
endmacro()

# Searches current directory for the sources and headers
macro( make_sources )
  file( GLOB ${PROJECT_NAME}_HEADERS *.hpp )
  file( GLOB ${PROJECT_NAME}_SOURCES *.cpp )
endmacro()

#Builds library using settings from configure_project() and find_prerequests()
macro( build_library )
  include_directories( ${${PROJECT_NAME}_PREREQUESTS_INCLUDE_DIR} ) 

  add_library( ${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} )
  target_link_libraries( ${PROJECT_NAME} ${${PROJECT_NAME}_PREREQUESTS_LIBRARIES} ) 

  copy_library( ${PROJECT_NAME} )

  if( ${PROJECT_NAME}_BUILD_EXAMPLES )
    set( LIBRARY_NAME ${PROJECT_NAME} )
    add_examples( ${PROJECT_NAME} )
  endif()

  install( TARGETS ${PROJECT_NAME} DESTINATION ${LIBRARY_INSTALL_PATH} )
  install( FILES ${${PROJECT_NAME}_HEADERS} DESTINATION ${INCLUDE_INSTALL_PATH} )

  find_package( GTest )
  if( GTEST_FOUND )
    file( GLOB TESTS_DIRECTORY tests )
    if( TESTS_DIRECTORY )
       file( GLOB TEST_SOURCES tests/*.cpp )

       include_directories( ${GTEST_INCLUDE_DIR} ${SOURCE_PATH} )
       add_executable( ${PROJECT_NAME}Test ${TEST_SOURCES} ${TEST_MAIN} )
       target_link_libraries( ${PROJECT_NAME}Test ${GTEST_LIBRARIES} ${PROJECT_NAME} )

       add_test( ${PROJECT_NAME}Test ${PROJECT_NAME}Test )

       copy_test( ${PROJECT_NAME}Test )

    endif()
  endif()
endmacro()

macro( make_library )
  make_sources()
  build_library()
endmacro()

# Builds example using results of find_prerequests and configure_example
macro( make_example )
  make_sources()
  include_directories( ${${PROJECT_NAME}_PREREQUESTS_INCLUDE_DIR} )

  add_executable ( ${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} )
  target_link_libraries ( ${PROJECT_NAME} ${${PROJECT_NAME}_PREREQUESTS_LIBRARIES} ) 

  copy_example( ${PROJECT_NAME} ${LIBRARY_NAME} )
 
  if( INSTALL_EXAMPLES )
    install( TARGETS ${PROJECT_NAME} DESTINATION ${BINARY_INSTALL_PATH}/examples/${LIBRARY_NAME}/${PROJECT_NAME} )
  endif()
endmacro()

#Build utility program using results of find_prerequests and configure_project
macro( make_util )
  make_sources()
  include_directories( ${${PROJECT_NAME}_PREREQUESTS_INCLUDE_DIR} )

  add_executable( ${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} )
  target_link_libraries( ${PROJECT_NAME} ${${PROJECT_NAME}_PREREQUESTS_LIBRARIES} )

  copy_binary( ${PROJECT_NAME} )

  install( TARGETS ${PROJECT_NAME} DESTINATION ${BINARY_INSTALL_PATH} )
endmacro()