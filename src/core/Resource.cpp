/**
 * @file Resource.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <iostream>
#include <chrono>

#include "Resource.hpp"
#include "QueryFactory.hpp"
#include "Request.hpp"
#include "Database.hpp"
#include "Logger.hpp"

using namespace httpserver;
using namespace std::chrono;

void Resource::render( const http_request& hreq, http_response** res )
{
  Logger &log = Logger::instance();
  log.info("A query is received");

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  Request req( hreq );
  std::string result( "" );
  int code( 200 );

  if ( req.is_valid() )
  {
    std::unique_ptr<Query> query = QueryFactory::make( req );

    Database db;
    if ( db.connect( "lids_toulouse_compressed_dimensional" ) )
    {
      if ( db.execute( *query ) )
      {
        result = query->result();
        log.info( "Query correctly processed" );
      }
      else
      {
        result = "Query executed on database has failed";
        code = 500;
        log.err( result );
      }
      db.disconnect();
    }
    else
    {
      result = "Connection to the database has failed";
      code = 500;
      log.err( result );
    }
  }
  else
  {
    result = "Invalid request";
    code = 500;
    log.err( result );
  }

  log.info( result );

  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>( t2 - t1 ).count();
  log.info( "Duration microsec: " + std::to_string(duration) );
  log.info( "Duration millisec: " + std::to_string(duration/1000) );
  log.info( "Duration sec: " + std::to_string(duration/1000/1000) );

  *res = new http_response(http_response_builder(result, code).with_header("Access-Control-Allow-Origin", "http://192.168.1.19:8083").string_response());
}
