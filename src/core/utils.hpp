/**
 * @file utils.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 27 Jun 2016
 */

#pragma UTILS_HPP

#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <jsoncpp/json/json.h>

// ----------------------------------------------------------------------------
// types
// ----------------------------------------------------------------------------
union UInt8Bytes
{
    uint8_t ival;
    uint8_t bytes[sizeof(uint8_t)];
};

union UInt16Bytes
{
    uint16_t ival;
    uint8_t bytes[sizeof(uint16_t)];
};

union Int32Bytes
{
    int32_t ival;
    uint8_t bytes[sizeof(int32_t)];
};

// ----------------------------------------------------------------------------
// functions
// ----------------------------------------------------------------------------
std::vector<std::string> split(const std::string &s, char delim);

bool export_to_wkt( int32_t epsg_code, std::string &wkt );

// ----------------------------------------------------------------------------
// BoundingBox
// ----------------------------------------------------------------------------
class Schema;
class BoundingBox
{
  public:
    BoundingBox();
    BoundingBox( float xmin, float xmax, float ymin, float ymax,
       float zmin, float zmax );
    BoundingBox( const std::string &xmin, const std::string &xmax,
        const std::string &ymin, const std::string &ymax,
        const std::string &zmin, const std::string &zmax );

    bool is_valid() const;
    std::string to_string() const; // pg geometry format
    Json::Value json_value() const;

    /* Returns a BoundingBox object from a string defined as
     * '[xmin, ymin, zmin, xmax, ymax, zmax]'
     * or 'xmin, ymin, zmin, xmax, ymax, zmax'
     */
    static BoundingBox from_string( const std::string &str, Schema &sch );

    std::string xmin_str() const;
    std::string xmax_str() const;
    std::string ymin_str() const;
    std::string ymax_str() const;
    std::string zmin_str() const;
    std::string zmax_str() const;

    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
};

// ----------------------------------------------------------------------------
// Dimension
// ----------------------------------------------------------------------------
class Dimension
{
  public:
    Dimension();
    Dimension( const std::string &name, const std::string type, size_t size );
    Json::Value json_value() const;

    /* Returns a dimension object from a json string defined as
     * '{name : ..., size : ..., type : ...}'
     * or 'name : ..., size : ..., type : ...'
     */
    static void from_pc_json( const std::string &json, Dimension &dim );

    static Dimension to_potree_dimension( const Dimension &dim );

    std::string name;
    std::string type;
    int32_t size;

  private:
    static std::string potree_type( const std::string &pc_type );
};

// ----------------------------------------------------------------------------
// Schema
// ----------------------------------------------------------------------------
class Schema
{
  public:
    static Schema from_json( const std::string &json );

    Json::Value json_value() const;
    bool is_defined( const std::string &dim ) const;
    size_t get_position( const std::string &dim_name ) const;

    size_t get_position_x() const;
    size_t get_position_y() const;
    size_t get_position_z() const;

    std::map<std::string, Dimension> dimensions;
    std::vector<std::string> dimensions_order;
};

/* Static schema asked by Potree within the Info query when "Greyhound" loader
 * is used
 */
class SchemaPotreeGreyhoundInfo : public Schema
{
  public:
    SchemaPotreeGreyhoundInfo();
};

/* Static schema asked by Potree within the Read query when "Greyhound" loader
 * is used
 */
class SchemaPotreeGreyhoundRead : public Schema
{
  public:
    SchemaPotreeGreyhoundRead();
};

// ----------------------------------------------------------------------------
// Point
// ----------------------------------------------------------------------------
class Point
{
  public:
    Point();
    static Point from_string( const std::string &ptstr, const Schema &sch );

    bool valid() const;

    std::vector<std::string> data; // not casted
    Schema schema; // data and schema.dimensions_order are well ordered
    double x;
    double y;
    double z;
};
