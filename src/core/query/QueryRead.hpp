/**
 * @file QueryRead.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#pragma once

#include "../Query.hpp"
#include "../Database.hpp"

class QueryRead : public Query
{
  public:
    QueryRead();
    ~QueryRead();

  protected:
    bool run( Database *db ) override;
    bool parse_params() override;

  private:
    void format_json_result( int npoints, const std::string &schema,
        const BoundingBox &box, const std::string &srs );

    BoundingBox _box;
    Schema _schema;
    int _lod_begin;
    int _lod_end;
};
