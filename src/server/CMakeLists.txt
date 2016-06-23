set ( LIGHTPCSS_SRC
  main.cpp )

include_directories ( ${CORE_DIR} )

add_executable ( lightpcss ${LIGHTPCSS_SRC} )
target_link_libraries ( lightpcss ${LIBHTTPSERVER_LIBRARIES} )
target_link_libraries ( lightpcss lightpcss_core )

install ( PROGRAMS lightpcss DESTINATION bin )
