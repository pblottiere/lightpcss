set ( CORE_SRC
  Query.cpp
  QueryFactory.cpp
  Resource.cpp
  Database.cpp
  Request.cpp
  query/QueryInfo.cpp )

add_library ( lightpcss_core SHARED ${CORE_SRC} )

target_link_libraries ( lightpcss_core ${POSTGRES_LIBRARY} )
