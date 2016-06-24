/**
 * @file QueryInfo.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#include <iostream>

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
  std::cout << "QueryInfo!!!!" << std::endl;
  std::cout << "Number of points: " << db->npoints() << std::endl;;
  return true;
}
