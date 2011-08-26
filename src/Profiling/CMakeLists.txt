project( Profiling )
cmake_minimum_required(VERSION 2.6)

find_prerequests( Profiling REQUIRED Boost GLOG Xml )
configure_project()
set( Profiling_BUILD_EXAMPLES OFF )
make_library()
target_link_libraries( Profiling rt )