/**
 * @file Request.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 24 Jun 2016
 */

#include <memory>
#include <iostream>

#include "Request.hpp"

Request::Request( const httpserver::http_request &req )
  : _valid( false )
  , _table( "" )
  , _query( "" )
  , _column( "" )
{
  // protocol: http://<IP>:<PORT>/resource/<TABLE>/<QUERY>
  int pieces_size = 4;

  if ( req.get_path_pieces_size() == pieces_size )
  {
    _table = req.get_path_piece(1);
    _column = req.get_path_piece(2);
    _query = req.get_path_piece(3);
    _valid = true;
  }
}

bool Request::is_valid() const
{
  return _valid;
}

std::string Request::table_name() const
{
  return _table;
}

std::string Request::query_name() const
{
  return _query;
}

std::string Request::column_name() const
{
  return _column;
}
