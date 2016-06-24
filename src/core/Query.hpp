/**
 * @file Query.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#pragma once

#include <string>

class Database;

class Query
{
  public:
    Query();
    ~Query();

    std::string result();

  protected:
    virtual bool run( Database *db );

    std::string _table;
    std::string _result;
    std::string _column;

    friend Database;
    friend class QueryFactory;
};
