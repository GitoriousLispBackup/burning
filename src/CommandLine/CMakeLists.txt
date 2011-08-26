project( CommandLine )
cmake_minimum_required(VERSION 2.6)

set( CommandLine_BOOST_COMPONENTS filesystem )
find_prerequests( CommandLine REQUIRED Boost GLOG )
configure_project()
set( CommandLine_BUILD_EXAMPLES OFF )
make_library()
