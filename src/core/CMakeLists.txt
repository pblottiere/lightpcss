set ( CORE_SRC
  Query.cpp
  QueryFactory.cpp
  Resource.cpp
  Database.cpp
  Request.cpp
  utils.cpp
  Config.cpp
  Logger.cpp
  Compressor.cpp
  query/QueryRead.cpp
  query/QueryHierarchy.cpp
  query/QueryInfo.cpp )

include_directories( ${POSTGRES_INCLUDE_DIR} )
include_directories( ${JSONCPP_INCLUDE_DIR} )
include_directories( ${GDAL_INCLUDE_DIR} )

add_library ( lightpcss_core SHARED ${CORE_SRC} )

target_link_libraries ( lightpcss_core ${POSTGRES_LIBRARY} )
target_link_libraries ( lightpcss_core ${JSONCPP_LIBRARY} )
target_link_libraries ( lightpcss_core ${GDAL_LIBRARY} )

install ( TARGETS lightpcss_core LIBRARY DESTINATION lib )
