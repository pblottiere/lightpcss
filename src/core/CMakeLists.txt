set ( CORE_SRC
  Query.cpp
  QueryFactory.cpp
  Resource.cpp
  query/QueryInfo.cpp )

add_library ( lightpcss_core SHARED ${CORE_SRC} )
