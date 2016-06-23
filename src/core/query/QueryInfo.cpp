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

void QueryInfo::execute()
{
  std::cout << "query info!" << std::endl;
}
