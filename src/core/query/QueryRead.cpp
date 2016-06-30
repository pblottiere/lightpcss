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
#include "../Compressor.hpp"
#include "../Logger.hpp"

const std::string PARAM_KEY_LOD_BEGIN = "depthBegin";
const std::string PARAM_KEY_LOD_END = "depthEnd";
const std::string PARAM_KEY_BOUNDS = "bounds";
const std::string PARAM_KEY_SCALE = "scale";
const std::string PARAM_KEY_OFFSET = "offset";
const std::string PARAM_KEY_COMPRESS = "compress";

QueryRead::QueryRead()
  : Query()
  , _schema ( SchemaPotreeGreyhoundRead() )
  , _lod_begin( 0 )
  , _lod_end( 0 )
  ,  _scale( 1.0 )
  , _compress( true )
{
  _content_type = "application/octet-stream";
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

    db->get_points( _box, 500000, points );

    _result = "";
    int32_t nof_valid_pts = 0;
    for ( size_t i=0; i<points.size(); i++ )
    {
      Point pt = points[i];

      if ( pt.valid() )
      {
        nof_valid_pts++;

        int32_t xval = ( pt.x - _offset.xmin ) / _scale;
        int32_t yval = ( pt.y - _offset.ymin ) / _scale;
        int32_t zval = ( pt.z - _offset.zmin ) / _scale;
        uint16_t ival = 0;
        uint8_t cval = 0;
        uint16_t rval = 0;
        uint16_t gval = 0;
        uint16_t bval = 0;

        add_dim_to_result( xval );
        add_dim_to_result( yval );
        add_dim_to_result( zval );
        add_dim_to_result( ival );
        add_dim_to_result( cval );
        add_dim_to_result( rval );
        add_dim_to_result( gval );
        add_dim_to_result( bval );
      }
    }

    // laz compression
    if ( _compress )
    {
      SchemaPotreeGreyhoundRead read;
      CompressorLAZ laz( read );
      std::vector<unsigned char> compressed;

      std::vector<unsigned char> v(_result.begin(), _result.end());
      laz.compress( v, compressed );

      _result = "";
      for ( size_t i=0; i<compressed.size(); i++ )
        _result.push_back( compressed[i] );
    }

    // footer
    if ( nof_valid_pts == 0 )
      _result = {0x00, 0x00, 0x00, 0x00};
    else
      add_dim_to_result( nof_valid_pts );

    log.info( "NP: " + std::to_string(nof_valid_pts) );
  }

  rc = true;

  return rc;
}

void QueryRead::add_dim_to_result( int32_t val )
{
  Int32Bytes bytes;
  bytes.ival = val;

  _result.push_back( bytes.bytes[0] );
  _result.push_back( bytes.bytes[1] );
  _result.push_back( bytes.bytes[2] );
  _result.push_back( bytes.bytes[3] );
}

void QueryRead::add_dim_to_result( uint16_t val )
{
  UInt16Bytes bytes;
  bytes.ival = val;

  _result.push_back( bytes.bytes[0] );
  _result.push_back( bytes.bytes[1] );
}

void QueryRead::add_dim_to_result( uint8_t val )
{
  UInt16Bytes bytes;
  bytes.ival = val;

  _result.push_back( bytes.bytes[0] );
}

bool QueryRead::parse_params()
{
  std::string key = "";

  key = PARAM_KEY_LOD_BEGIN;
  if ( _params.find(key) != _params.end() )
    _lod_begin = std::stoi(_params[key]);
  else
    goto err;

  key = PARAM_KEY_LOD_END;
  if ( _params.find(key) != _params.end() )
    _lod_end = std::stoi(_params[key]);
  else
    goto err;

  key = PARAM_KEY_BOUNDS;
  if ( _params.find(key) != _params.end() )
  {
    SchemaPotreeGreyhoundRead sch;
    _box = BoundingBox::from_string( _params[key], sch );
  }
  else
    goto err;

  key = PARAM_KEY_SCALE;
  if ( _params.find(key) != _params.end() )
    _scale = atof( _params[key].c_str() );
  else
    goto err;

  key = PARAM_KEY_OFFSET;
  if ( _params.find(key) != _params.end() )
  {
    SchemaPotreeGreyhoundRead sch;
    _offset = BoundingBox::from_string( _params[key], sch );
  }
  else
    goto err;

  key = PARAM_KEY_COMPRESS;
  if ( _params.find(key) != _params.end() )
  {
    if ( _params[key] == "true" )
      _compress = true;
    else
      _compress = false;
  }
  else
    goto err;

  return true;

err:
  Logger::instance().err( "Invalid parameter '" + key + "'" );
  return false;
}
