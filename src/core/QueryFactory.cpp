/**
 * @file QueryFactory.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include "QueryFactory.hpp"
#include "query/QueryInfo.hpp"

std::unique_ptr<Query> QueryFactory::make( const Request &req )
{
  std::unique_ptr<Query> query( new Query );

  if ( req.is_valid() )
  {

    if ( req.query_name() == "info" )
    {
      query.reset( new QueryInfo );
    }
    else
    {
      // throw exception!
    }

    query->_table = req.table_name();
    query->_column = req.column_name();
  }

  return query;
}
