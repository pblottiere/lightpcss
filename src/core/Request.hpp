/**
 * @file Request.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 24 Jun 2016
 */

#pragma once

#include <httpserver.hpp>

class Request
{
  public:
    Request( const httpserver::http_request &req );

    bool is_valid() const;
    std::string table_name() const;
    std::string query_name() const;
    std::string column_name() const;
    void params( std::map<std::string, std::string,
        httpserver::http::arg_comparator> &params ) const;

  private:
    bool _valid;
    std::string _table;
    std::string _column;
    std::string _query;
    std::map<std::string, std::string, httpserver::http::arg_comparator> _params;
};
