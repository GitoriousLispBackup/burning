macro( create_directory name )
  file( MAKE_DIRECTORY ${BASE_PATH}/bin/${name} )
endmacro()

macro( copy_file name destination )

  get_target_property( FILENAME ${name} LOCATION )
  add_custom_command( TARGET ${name} POST_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy ${FILENAME} ${destination}/ )
endmacro()

macro( copy_example name project )
  copy_file( ${name} ${EXAMPLES_BINARY_PATH}/${project} )
endmacro()

macro( copy_library name )
  copy_file( ${name} ${LIBRARY_PATH} )
endmacro()

macro( copy_binary name )
  copy_file( ${name} ${BINARY_PATH} )
endmacro()

macro( copy_test name )
  copy_file( ${name} ${BINARY_PATH}/tests )
endmacro()

macro( add_examples name )
  create_directory( examples/${name} )
  add_subdirectory( ${EXAMPLES_PATH}/${name} ${CMAKE_CURRENT_BINARY_DIR}/examples )
endmacro()
