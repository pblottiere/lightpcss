/**
 * @file Database.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 24 Jun 2016
 */

#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

#include "Database.hpp"

Database::Database()
  : _res( NULL )
{
}

Database::~Database()
{
  disconnect();
}

bool Database::connect( const std::string &dbname )
{
  std::string conninfo = "dbname = " + dbname;
  _conn = PQconnectdb( conninfo.c_str() );
  return is_connected();
}

bool Database::disconnect()
{
  if ( is_connected() )
    PQfinish( _conn );

  return ! is_connected();
}

bool Database::is_connected() const
{
  bool rc = false;

  if ( PQstatus( _conn ) == CONNECTION_OK )
    rc = true;

  return rc;
}

bool Database::execute( Query &query )
{
  // keep table and column
  _table = query._table;
  _column = query._column;

  // return validity of the result
  return query.run( this );
}

int32_t Database::npoints()
{
  int32_t npoints = -1;
  std::string sql = "select sum(pc_numpoints(" + _column + ")) from "
    + _table + ";";

  if ( get_res( sql ) )
  {
    char *nstr = PQgetvalue( _res, 0, 0 ) ;
    npoints = atoi( nstr );
  }

  clear_res();

  return npoints;
}

std::string Database::schema()
{
  std::string sql = "select json_array_elements(PC_Summary("
    + _column + ")::json->'dims') from "
    + _table
    + " where id = 1;";

  Json::FastWriter writer;
  Json::Value root_schema;
  if ( get_res( sql ) )
  {
    int nfields = PQnfields( _res );
    for ( int i = 0; i < PQntuples(_res); i++)
    {
      for (int j = 0; j < nfields; j++)
      {
        std::string dim = PQgetvalue(_res, i, j);

        Json::Value root_schema_dim;
        Json::Value root_dim;
        Json::Reader reader_dim;
        if ( reader_dim.parse( dim, root_dim ) )
        {
          std::string name = root_dim.get("name", "").asString();
          std::string size = root_dim.get("size", "").asString();
          std::string type = root_dim.get("type", "").asString();

          root_schema_dim["name"] = name;
          root_schema_dim["size"] = size;
          root_schema_dim["type"] = type;

          root_schema.append( writer.write( root_schema_dim ) );
        }
      }
    }
  }

  clear_res();

  return writer.write( root_schema );
}

bool Database::get_res( const std::string &sql )
{
  bool rc = false;

  clear_res();

  if ( is_connected() )
  {
    _res = PQexec( _conn, sql.c_str() );
    if ( PQresultStatus( _res ) == PGRES_TUPLES_OK )
      rc = true;
    else
      clear_res();
  }

  return rc;
}

void Database::clear_res()
{
  if ( _res != nullptr )
  {
    PQclear( _res );
    _res = nullptr;
  }
}
