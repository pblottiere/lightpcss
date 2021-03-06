/**
 * @file Database.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 24 Jun 2016
 */

#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

#include "Logger.hpp"
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

int32_t Database::npoints( const BoundingBox &box )
{
  int32_t npoints = -1;
  std::string poly = box.to_string();
  std::string srsid = std::to_string(srs_id());
  std::string sql = "select sum(pc_numpoints("
    + _column + ")) as pt from "
    + _table + " where pc_intersects("
    + _column + ", st_geomfromtext('polygon (("
    + poly + "))',"
    + srsid + "));";

  if ( get_res( sql ) )
  {
    char *nstr = PQgetvalue( _res, 0, 0 ) ;
    npoints = atoi( nstr );
  }

  clear_res();

  return npoints;
}

Schema Database::schema()
{
  std::string sql = "select json_array_elements(PC_Summary("
    + _column + ")::json->'dims') from "
    + _table
    + " where id = 1;";

  Schema schema;
  if ( get_res( sql ) )
  {
    int nfields = PQnfields( _res );
    for ( int i = 0; i < PQntuples(_res); i++)
    {
      for (int j = 0; j < nfields; j++)
      {
        std::string dimjson = PQgetvalue(_res, i, j);

        Dimension pc_dim;
        Dimension::from_pc_json( dimjson, pc_dim );

        Dimension potree_dim = Dimension::to_potree_dimension( pc_dim );
        schema.dimensions[potree_dim.name] = potree_dim;
        schema.dimensions_order.push_back( potree_dim.name );
      }
    }
  }

  clear_res();

  return schema;
}

bool Database::bounding_box( BoundingBox &box )
{
  bool rc = false;

  std::string sql = "select min( pc_patchmin(" + _column + ", 'y') ) as ymin"
    + ", max( pc_patchmax(" + _column + ", 'y') ) as ymax"
    + ", min( pc_patchmin(" + _column + ", 'x') ) as xmin"
    + ", max( pc_patchmax(" + _column + ", 'x') ) as xmax"
    + ", min( pc_patchmin(" + _column + ", 'z') ) as zmin"
    + ", max( pc_patchmax(" + _column + ", 'z') ) as zmax"
    + " from " + _table + ";";

  if ( get_res( sql ) )
  {
    int nfields = PQnfields( _res );

    // all fields are available
    if ( nfields == 6 )
    {
      std::string xmin = PQgetvalue( _res, 0, 2 );
      std::string xmax = PQgetvalue( _res, 0, 3 );
      std::string ymin = PQgetvalue( _res, 0, 0 );
      std::string ymax = PQgetvalue( _res, 0, 1 );
      std::string zmin = PQgetvalue( _res, 0, 4 );
      std::string zmax = PQgetvalue( _res, 0, 5 );

      box = BoundingBox( xmin, xmax, ymin, ymax, zmin, zmax );

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

bool Database::get_points( const BoundingBox &box, int32_t npoints,
    GET_METHOD method, std::vector<Point> &points )
{
  bool rc = false;

  Schema pc_sch = schema();
  std::string lod = std::to_string(npoints);
  std::string srsid = std::to_string(srs_id());
  std::string poly = box.to_string();

  std::string sql;
  if ( method == GET_METHOD::FIRST)
  {
    sql = "select pc_get(pc_explode("
      + _column + ")) as pt from "
      + _table + " where pc_intersects("
      + _column + ", st_geomfromtext('polygon (("
      + poly + "))',"
      + srsid + ")) limit " + lod + ";";

    if ( get_res( sql ) )
    {
      int nfields = PQnfields( _res );
      for ( int i = 0; i < PQntuples(_res); i++)
      {
        for (int j = 0; j < nfields; j++)
        {
          std::string ptstr = PQgetvalue(_res, i, j);
          Point pt = Point::from_string( ptstr, pc_sch );
          points.push_back( pt );
        }
      }
    }
    clear_res();
  }
  else if ( method == GET_METHOD::SECOND )
  {
    for ( int32_t j = 0; j< npoints; j++ )
      get_pointn( box, j+1, points );
  }
  else if ( method == GET_METHOD::THIRD )
  {
    size_t patch_size = 400; // TODO: autodetect
    for ( int32_t j = 0; j< npoints; j++ )
    {
      int32_t r = rand() % patch_size;
      get_pointn( box, r, points );
    }
  }

  return rc;
}

bool Database::get_pointn( const BoundingBox &box, int32_t n, std::vector<Point> &pts )
{
  bool rc = false;

  Schema pc_sch = schema();
  std::string n_str = std::to_string( n );
  std::string srsid = std::to_string(srs_id());
  std::string poly = box.to_string();

  std::string sql = "select pc_get(pc_pointn("
    + _column + ", " + n_str + ")) as pt from "
    + _table + " where pc_intersects("
    + _column + ", st_geomfromtext('polygon (("
    + poly + "))',"
    + srsid + "));";

  if ( get_res( sql ) )
  {
    int nfields = PQnfields( _res );
    for ( int i = 0; i < PQntuples(_res); i++)
    {
      for (int j = 0; j < nfields; j++)
      {
        std::string ptstr = PQgetvalue(_res, i, j);
        if ( !ptstr.empty() )
        {
          Point pt = Point::from_string( ptstr, pc_sch );
          pts.push_back( pt );
        }
      }
    }
  }
  clear_res();

  return rc;
}

bool Database::get_res( const std::string &sql )
{
  Logger &log = Logger::instance();
  log.info( "Run SQL on database: " + sql );

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
