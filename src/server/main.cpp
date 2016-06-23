/**
 * @file main.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <iostream>
#include <stdint.h>

#include <httpserver.hpp>
#include <Resource.hpp>

using namespace httpserver;

int main()
{
  // config
  int32_t port = 8080;
  int32_t max_threads = 5;

  // create web server
  create_webserver cws = create_webserver(port);
  cws.start_method(http::http_utils::INTERNAL_SELECT);
  cws.max_threads(max_threads);

  webserver ws = cws;

  // init resource
  Resource pcr;
  ws.register_resource("/resource", &pcr, true);

  // start server (blocking call)
  ws.start(true);

  return 0;
}
