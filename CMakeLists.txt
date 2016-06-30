#------------------------------------------------------------------------------
# BASICS
#------------------------------------------------------------------------------
cmake_minimum_required ( VERSION 2.6 )
project ( lightpcss )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-unknown-pragmas")

#------------------------------------------------------------------------------
# ENV VAR
#------------------------------------------------------------------------------
set (CORE_DIR ${PROJECT_SOURCE_DIR}/src/core)

#------------------------------------------------------------------------------
# CHECK REQUIRED LIBS
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package ( LibHttpServer REQUIRED )
find_package ( Postgres REQUIRED )
find_package ( Jsoncpp REQUIRED )
find_package ( LazPerf REQUIRED )
find_package ( GDAL REQUIRED )

#------------------------------------------------------------------------------
# custom commands
#------------------------------------------------------------------------------
add_custom_target( uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake" )

#------------------------------------------------------------------------------
# SUBDIRS
#------------------------------------------------------------------------------
add_subdirectory ( src )
add_subdirectory ( etc )
