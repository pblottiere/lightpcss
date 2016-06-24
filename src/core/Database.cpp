/**
 * @file Database.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 24 Jun 2016
 */

#include <iostream>

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
