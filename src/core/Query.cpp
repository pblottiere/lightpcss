/**
 * @file Query.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include "Query.hpp"

Query::Query()
  : _result("")
  , _database("")
  , _is_valid(false)
{
}

Query::~Query()
{
}

void Query::execute()
{
}

std::string Query::result()
{
  return _result;
}

bool Query::is_valid()
{
  return _is_valid;
}
