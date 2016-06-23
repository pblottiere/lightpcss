/**
 * @file QueryFactory.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include "QueryFactory.hpp"
#include "query/QueryInfo.hpp"

std::unique_ptr<Query> QueryFactory::make( const httpserver::http_request &req )
{
  // protocol: http://<IP>:<PORT>/resource/<DB>/<QUERY>
  int pieces_size = 3;
  std::unique_ptr<Query> query( new Query );

  if ( req.get_path_pieces_size() == pieces_size )
  {
    std::string database = req.get_path_piece(1);
    std::string name = req.get_path_piece(2);

    if ( name == "info" )
    {
      query.reset( new QueryInfo );
      query->_is_valid = true;
    }
    else
    {
      // TODO: query->_result = ERROR!!!!!!
      query->_is_valid = true;
    }

    query->_database = database;
  }

  return query;
}
