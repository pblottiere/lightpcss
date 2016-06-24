/**
 * @file QueryInfo.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <iostream>
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

  if ( npoints != -1 )
  {
    format_json_result( npoints );
    rc = true;
  }

  return true;
}

void QueryInfo::format_json_result( int npoints )
{
  Json::Value root;
  root["numPoints"] = npoints;

  Json::StyledWriter writer;
  _result = writer.write( root );
}
