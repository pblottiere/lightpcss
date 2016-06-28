/**
 * @file QueryRead.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <jsoncpp/json/json.h>

#include "QueryRead.hpp"
#include "../Logger.hpp"

/* GET Request
GET /resource/terrain/read?depthBegin=10&depthEnd=11&bounds=[728630,4676917,309,728820,4677107,499]&schema=[{name:X,size:4,type:signed},{name:Y,size:4,type:signed},{name:Z,size:4,type:signed},{name:Intensity,size:2,type:unsigned},{name:Classification,size:1,type:unsigned},{name:Red,size:2,type:unsigned},{name:Green,size:2,type:unsigned},{name:Blue,size:2,type:unsigned}]&scale=0.01&offset=[728630,4676727,309]&compress=true
 */

/* Result waited
 */

QueryRead::QueryRead()
  : Query()
  , _lod_begin( 0 )
  , _lod_end( 0 )
{
}

QueryRead::~QueryRead()
{
}

bool QueryRead::run( Database *db )
{
  Logger &log = Logger::instance();
  bool rc = false;

  if ( parse_params() )
  {
    std::vector<Point> points;
    db->get_points( _box, points );
    log.info( "NP: " + std::to_string(points.size()) );

    std::vector<unsigned char> bytes;
    for ( size_t j=0; j<points.size(); j++ )
    {
      Point pt = points[j];

      for ( size_t i=0; i<_schema.dimensions_order.size(); i++ )
      {
        std::vector<unsigned char> ptbytes;
        std::string dimname = _schema.dimensions_order[i];
        Dimension d = _schema.dimensions[dimname];
        ptbytes = pt.get_dim_hex( d );
      }
    }
  }

  return rc;
}

bool QueryRead::parse_params()
{
  Logger &logger = Logger::instance();
  bool lod_begin_ok = false;
  bool lod_end_ok = false;
  bool box_ok = false;
  bool schema_ok = false;

  std::string lod_begin_key = "depthBegin";
  if ( _params.find(lod_begin_key) != _params.end() )
  {
    _lod_begin = std::stoi(_params[lod_begin_key]);
    lod_begin_ok = true;
  }
  else
    logger.err( "Invalid 'depthBegin' parameter" );

  std::string lod_end_key = "depthEnd";
  if ( _params.find(lod_end_key) != _params.end() )
  {
    _lod_end = std::stoi(_params[lod_end_key]);
    lod_end_ok = true;
  }
  else
    logger.err( "Invalid 'depthEnd' parameter" );

  std::string bounds_key = "bounds";
  if ( _params.find(bounds_key) != _params.end() )
  {
    _box = BoundingBox::from_string( _params[bounds_key] );
    box_ok = _box.is_valid();
  }
  else
    logger.err( "Invalid 'bounds' parameter" );

  std::string schema_key = "schema";
  if ( _params.find( schema_key ) != _params.end() )
  {
    _schema = Schema::from_json( _params[schema_key] );
    schema_ok = true;
  }

  return lod_end_ok && lod_begin_ok && box_ok && schema_ok;
}
