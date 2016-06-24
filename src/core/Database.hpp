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

struct BoundingBox
{
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  float zmax;
};

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
    bool bounding_box( BoundingBox &box );

  private:
    bool get_res( const std::string &sql );
    void clear_res();
    std::string potree_type( const std::string &pc_type );

    std::string _table;
    std::string _column;
    PGconn *_conn;
    PGresult *_res;
};
