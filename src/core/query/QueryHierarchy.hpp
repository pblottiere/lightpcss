/**
 * @file QueryHierarchy.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 28 Jun 2016
 */

#pragma once

#include "../Query.hpp"
#include "../Database.hpp"

class QueryHierarchy : public Query
{
  public:
    QueryHierarchy();
    ~QueryHierarchy();

  protected:
    bool run( Database *db ) override;
};
