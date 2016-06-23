/**
 * @file Resource.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include "Resource.hpp"
#include "QueryFactory.hpp"

using namespace httpserver;

void Resource::render( const http_request& req, http_response** res )
{
  std::unique_ptr<Query> query = QueryFactory::make( req );

  if ( query->is_valid() )
  {
    query->execute();
  }

  //*res = new http_response(http_response_builder("todo", 200).string_response());
}
