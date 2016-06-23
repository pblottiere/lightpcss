/**
 * @file QueryInfo.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#pragma once

#include "../Query.hpp"

class QueryInfo : public Query
{
  public:
    QueryInfo();
    ~QueryInfo();

    void execute() override;
};
