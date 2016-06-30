/**
 * @file QueryHierarchy.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 28 Jun 2016
 */

#include "QueryHierarchy.hpp"

Json::Value part(int rec, int rec_end)
{
  Json::Value v;

  rec++;
  if ( rec != rec_end )
  {
    v["n"] = 500000;

    if ( rec != (rec_end-1) )
    {
      v["nwu"] = part(rec, rec_end);
      v["nwd"] = part(rec, rec_end);
      v["neu"] = part(rec, rec_end);
      v["ned"] = part(rec, rec_end);
      v["swu"] = part(rec, rec_end);
      v["swd"] = part(rec, rec_end);
      v["seu"] = part(rec, rec_end);
      v["sed"] = part(rec, rec_end);
    }
  }

  return v;
}

std::string build_fake_hierarchy()
{
  Json::Value h = part( 0, 6 );
  Json::StyledWriter writer;
  return writer.write( h );
}

QueryHierarchy::QueryHierarchy()
  : Query()
{
}

QueryHierarchy::~QueryHierarchy()
{
}

bool QueryHierarchy::run( Database *db )
{
  bool rc = true;
  _result = build_fake_hierarchy();
  return rc;
}
