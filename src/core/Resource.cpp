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

  if ( req.is_valid() )
  {
    std::unique_ptr<Query> query = QueryFactory::make( req );

    Database db;
    if ( db.connect( "lids_toulouse_compressed_dimensional" ) )
    {
      db.execute( *query );
      db.disconnect();
    }
    else
    {
      std::cout << "Connection failed!" << std::endl;
      // return an error
    }
  }
  else
  {
    std::cout << "Invalid request!" << std::endl;
    // return an error
  }

  //*res = new http_response(http_response_builder("todo", 200).string_response());
}
