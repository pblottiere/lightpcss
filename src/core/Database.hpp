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
#include "utils.hpp"

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
    int32_t npoints( const BoundingBox &box );
    Schema schema();
    bool bounding_box( BoundingBox &box );
    int32_t srs_id();
    bool get_points( const BoundingBox &box, int32_t npoints,
        std::vector<Point> &points );

  private:
    bool get_res( const std::string &sql );
    void clear_res();

    std::string _table;
    std::string _column;
    PGconn *_conn;
    PGresult *_res;
};
