/**
 * @file Query.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <string>

#include "Query.hpp"

Query::Query()
  : _result("")
{
}

Query::~Query()
{
}

std::string Query::result()
{
  return _result;
}

bool Query::run( Database *db )
{
  return false;
}

bool Query::parse_params()
{
  return false;
}
