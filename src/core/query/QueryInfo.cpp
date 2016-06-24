/**
 * @file QueryInfo.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <iostream>
#include <algorithm>
#include <jsoncpp/json/json.h>

#include "QueryInfo.hpp"

/* JSON result waited
   {
     "baseDepth":6,
     "bounds":[728250,4676347,-71,729010,4677107,689],
     "boundsConforming":[728260.9648480237,4676437.915124173,292.56188993530276,728999.9735080238,4677016.123784173,327.1640399353027],
     "numPoints":3811489,
     "schema":[
     {"name":"X","size":8,"type":"floating"},
     {"name":"Y","size":8,"type":"floating"},
     {"name":"Z","size":8,"type":"floating"},
     {"name":"Intensity","size":2,"type":"unsigned"},
     {"name":"ReturnNumber","size":1,"type":"unsigned"},
     {"name":"NumberOfReturns","size":1,"type":"unsigned"},
     {"name":"ScanDirectionFlag","size":1,"type":"unsigned"},
     {"name":"EdgeOfFlightLine","size":1,"type":"unsigned"},
     {"name":"Classification","size":1,"type":"unsigned"},
     {"name":"ScanAngleRank","size":4,"type":"floating"},
     {"name":"UserData","size":1,"type":"unsigned"},
     {"name":"PointSourceId","size":2,"type":"unsigned"},
     {"name":"Red","size":2,"type":"unsigned"},
     {"name":"Green","size":2,"type":"unsigned"},
     {"name":"Blue","size":2,"type":"unsigned"},
     {"name":"Origin","size":4,"type":"unsigned"}],
     "srs":"PROJCS[\"WGS 84 / UTM zone 16N\",GEOGCS[\"WGS 84........]",
     "type":"octree"}
 */

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
    std::string schema = db->schema();
    BoundingBox box;
    bool box_ok = db->bounding_box( box );
    std::string srs = db->srs();

    if ( schema.size() > 0 && box_ok && srs.size() > 0 )
    {
      format_json_result( npoints, schema, box, srs );
      rc = true;
    }
  }

  return rc;
}

void QueryInfo::format_json_result( int npoints, const std::string &schema,
   const BoundingBox &box, const std::string &srs )
{
  Json::FastWriter writer;
  Json::Value root;

  Json::Value root_bounds;
  root_bounds.append( box.xmin );
  root_bounds.append( box.xmax );
  root_bounds.append( box.ymin );
  root_bounds.append( box.ymax );
  root_bounds.append( box.zmax );
  root["bounds"] = root_bounds;

  root["numPoints"] = npoints;
  root["schema"] = schema;

  root["srs"] = srs;

  root["type"] = "octree";

  _result = writer.write( root );
  clean_json( _result );
}
