/**
 * @file Query.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#pragma once

#include <string>
#include <map>

#include <httpserver.hpp>

class Database;

class Query
{
  public:
    Query();
    ~Query();

    std::string result();

  protected:
    virtual bool run( Database *db );
    virtual bool parse_params();
    void clean_json( std::string &json );

    std::string _table;
    std::string _result;
    std::string _column;
    std::map<std::string, std::string, httpserver::http::arg_comparator> _params;

    friend Database;
    friend class QueryFactory;
};
