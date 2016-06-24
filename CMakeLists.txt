#------------------------------------------------------------------------------
# BASICS
#------------------------------------------------------------------------------
cmake_minimum_required ( VERSION 2.6 )
project ( lightpcss )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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

#------------------------------------------------------------------------------
# SUBDIRS
#------------------------------------------------------------------------------
add_subdirectory ( src )
