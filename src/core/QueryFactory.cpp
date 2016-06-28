/**
 * @file QueryFactory.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include "QueryFactory.hpp"
#include "query/QueryInfo.hpp"
#include "query/QueryRead.hpp"
#include "Logger.hpp"

std::unique_ptr<Query> QueryFactory::make( const Request &req )
{
  Logger &logger = Logger::instance();

  std::unique_ptr<Query> query( new Query );

  if ( req.is_valid() )
  {
    if ( req.query_name() == "info" )
    {
      query.reset( new QueryInfo );
      logger.info( "Query type: Info" );
    }
    else if ( req.query_name() == "read" )
    {
      query.reset( new QueryRead );
      logger.info( "Query type: Read" );

      std::map<std::string, std::string, httpserver::http::arg_comparator> par;
      req.params( par );
      query->_params = par;
    }
    else
    {
      logger.err( "Query type: Invalid" );
      // throw exception!
    }

    query->_table = req.table_name();
    query->_column = req.column_name();
  }

  return query;
}
