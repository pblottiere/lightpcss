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

void Query::clean_json( std::string &json )
{
  std::string::size_type i;

  for ( i = json.find("\\n"); i != std::string::npos; i = json.find("\\n"))
    json.erase( i, 2 );

  for ( i = json.find("\\"); i != std::string::npos; i = json.find("\\"))
    json.erase( i, 1 );

  for ( i = json.find("\n"); i != std::string::npos; i = json.find("\n"))
    json.erase( i, 1 );
}
