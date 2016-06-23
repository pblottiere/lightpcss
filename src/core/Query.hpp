/**
 * @file Query.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#pragma once

#include <string>

class Query
{
  public:
    Query();
    ~Query();

    virtual void execute();
    std::string result();

    /**
     * Returns the semantic validity of the query
     */
    bool is_valid();

  private:
    std::string _result;
    std::string _database;
    bool _is_valid;

  friend class QueryFactory;
};
