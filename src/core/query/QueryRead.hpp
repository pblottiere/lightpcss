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
    void add_dim_to_result( int32_t val );
    void add_dim_to_result( uint16_t val );
    void add_dim_to_result( uint8_t val );

    void format_json_result( int npoints, const std::string &schema,
        const BoundingBox &box, const std::string &srs );

    BoundingBox _box;
    BoundingBox _offset; // defines min values
    Schema _schema;
    int32_t _lod_begin;
    int32_t _lod_end;
    float _scale;
    bool _compress;
};
