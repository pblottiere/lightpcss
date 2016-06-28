/**
 * @file QueryHierarchy.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 28 Jun 2016
 */

#include "QueryHierarchy.hpp"

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

  // empty response for now, just to see if potre is complaining...
  _result = "{}";

  return rc;
}
