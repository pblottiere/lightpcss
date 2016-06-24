/**
 * @file Resource.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <iostream>

#include "Resource.hpp"
#include "QueryFactory.hpp"
#include "Request.hpp"
#include "Database.hpp"

using namespace httpserver;

void Resource::render( const http_request& hreq, http_response** res )
{
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
        result = query->result();
      else
      {
        result = "Query failed";
        code = 500;
      }
      db.disconnect();
    }
    else
    {
      result = "Connection failed";
      code = 500;
    }
  }
  else
  {
    result = "Invalid request";
    code = 500;
  }

  *res = new http_response(http_response_builder(result, code).string_response());
}
