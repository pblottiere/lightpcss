/**
 * @file main.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <iostream>
#include <stdint.h>

#include <httpserver.hpp>
#include <Resource.hpp>
#include <Config.hpp>

using namespace httpserver;

int main()
{
  // config
  Config &cfg = Config::instance();

  // create web server
  create_webserver cws = create_webserver( cfg.port );
  cws.start_method(http::http_utils::INTERNAL_SELECT);
  cws.max_threads( cfg.max_threads );

  webserver ws = cws;

  // init resource
  Resource pcr;
  ws.register_resource( cfg.resource, &pcr, true );

  // start server (blocking call)
  ws.start(true);

  return 0;
}
