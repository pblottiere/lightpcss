/**
 * @file QueryInfo.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#pragma once

#include "../Query.hpp"
#include "../Database.hpp"

class QueryInfo : public Query
{
  public:
    QueryInfo();
    ~QueryInfo();

  protected:
    bool run( Database *db ) override;

  private:
    void format_json_result( int npoints );
};
