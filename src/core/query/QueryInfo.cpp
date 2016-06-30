/**
 * @file QueryInfo.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <iostream>
#include <algorithm>
#include <jsoncpp/json/json.h>

#include "QueryInfo.hpp"

QueryInfo::QueryInfo() :
  Query()
{
}

QueryInfo::~QueryInfo()
{
}

bool QueryInfo::run( Database *db )
{
  SchemaPotreeGreyhoundInfo schema;
  BoundingBox box;
  int32_t npoints = -1;
  int32_t srsid;
  std::string srs;

  if ( ( npoints = db->npoints() ) <= 0 )
    goto err;

  if ( ! db->bounding_box( box ) )
    goto err;

  if ( (srsid = db->srs_id()) == -1)
    goto err;

  export_to_wkt( srsid, srs );
  if ( srs.empty() )
    goto err;

  format_json_result( npoints, schema, box, srs );

  return true;

err:
  return false;
}

void QueryInfo::format_json_result( int npoints, const Schema &schema,
   const BoundingBox &box, const std::string &srs )
{
  Json::StyledWriter writer;
  Json::Value root;

  root["baseDepth"] = 6;
  root["bounds"] = box.json_value();
  root["boundsConforming"] = box.json_value();
  root["numPoints"] = npoints;
  root["schema"] = schema.json_value();
  root["srs"] = srs;
  root["type"] = "octree";

  _result = writer.write( root );
}
