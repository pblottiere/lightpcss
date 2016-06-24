/**
 * @file Database.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 24 Jun 2016
 */

#pragma once

#include <string>
#include <memory>
#include <libpq-fe.h>

#include "Query.hpp"

class Database
{
  public:
    Database();
    ~Database();

    bool is_connected() const;
    bool connect( const std::string &dbname );
    bool disconnect();
    bool execute( Query &query );

    int32_t npoints();
    std::string schema();

  private:
    bool get_res( const std::string &sql );
    void clear_res();

    std::string _table;
    std::string _column;
    PGconn *_conn;
    PGresult *_res;
};
