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
  bool rc = false;

  int npoints = db->npoints();

  if ( npoints > 0 )
  {
    SchemaPotreeGreyhound schema;
    BoundingBox box;
    bool box_ok = db->bounding_box( box );
    std::string srs = db->srs();

    if ( box_ok && srs.size() > 0 )
    {
      format_json_result( npoints, schema, box, srs );
      rc = true;
    }
  }

  return rc;
}

void QueryInfo::format_json_result( int npoints, const Schema &schema,
   const BoundingBox &box, const std::string &srs )
{
  Json::FastWriter writer;
  Json::Value root;

  root["bounds"] = box.json_value();
  root["boundsConforming"] = box.json_value();

  root["numPoints"] = npoints;
  root["schema"] = schema.json_value();

  root["srs"] = srs;

  root["type"] = "octree";

  _result = writer.write( root );
}
