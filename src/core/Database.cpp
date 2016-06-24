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
          int32_t size = root_dim.get("size", "").asInt();
          std::string type = root_dim.get("type", "").asString();

          root_schema_dim["name"] = name;
          root_schema_dim["size"] = size;
          root_schema_dim["type"] = potree_type(type);

          root_schema.append( writer.write( root_schema_dim ) );
        }
      }
    }
  }

  clear_res();

  return writer.write( root_schema );
}

std::string Database::potree_type( const std::string &pc_type )
{
  // TODO : to complete
  std::string potree_type = "signed";

  if ( pc_type == "double" )
    potree_type = "floating";
  else if ( pc_type == "int32_t" )
    potree_type = "signed";

  return potree_type;
}

bool Database::bounding_box( BoundingBox &box )
{
  bool rc = false;

  std::string sql = "select min( pc_patchmin(" + _column + ", 'y') ) as ymin"
    + ", max( pc_patchmax(" + _column + ", 'y') ) as ymax"
    + ", min( pc_patchmin(" + _column + ", 'x') ) as xmin"
    + ", max( pc_patchmax(" + _column + ", 'x') ) as xmax"
    + ", max( pc_patchmax(" + _column + ", 'z') ) as zmax"
    + " from " + _table + ";";

  if ( get_res( sql ) )
  {
    int nfields = PQnfields( _res );
    if ( nfields == 5 )
    {
      std::string xmin = PQgetvalue( _res, 0, 0 );
      std::string xmax = PQgetvalue( _res, 0, 1 );
      std::string ymin = PQgetvalue( _res, 0, 2 );
      std::string ymax = PQgetvalue( _res, 0, 3 );
      std::string zmax = PQgetvalue( _res, 0, 4 );

      box.xmin = atof( xmin.c_str() );
      box.xmax = atof( xmax.c_str() );
      box.ymin = atof( ymin.c_str() );
      box.ymax = atof( ymax.c_str() );
      box.zmax = atof( zmax.c_str() );

      rc = true;
    }
  }

  clear_res();

  return rc;
}

int32_t Database::srs_id()
{
  std::string sql = "select PC_Summary("
    + _column + ")::json->'srid' from "
    + _table + " where id = 1;";

  int id = -1;
  if ( get_res( sql ) )
  {
    char *id_str = PQgetvalue( _res, 0, 0 ) ;
    id = atoi( id_str );
  }
  clear_res();

  return id;
}

std::string Database::srs()
{
  std::string srs_str;
  std::string sql = "select srtext from spatial_ref_sys where srid = "
    + std::to_string( srs_id() ) + ";";

  if ( get_res( sql ) )
  {
    char *srs = PQgetvalue( _res, 0, 0 ) ;
    srs_str = srs;
  }
  clear_res();

  return srs_str;
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
