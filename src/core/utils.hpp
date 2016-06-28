/**
 * @file utils.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 27 Jun 2016
 */

#pragma UTILS_HPP

#include <string>
#include <vector>
#include <map>
#include <jsoncpp/json/json.h>

std::vector<std::string> split(const std::string &s, char delim);

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
    static BoundingBox from_string( const std::string &str );

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

class Dimension
{
  public:
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

class Schema
{
  public:
    static Schema from_json( const std::string &json );

    Json::Value json_value() const;
    bool is_defined( const std::string &dim ) const;

    std::map<std::string, Dimension> dimensions;
    std::vector<std::string> dimensions_order;
};

class Point
{
  public:
    // {"pcid":1,
    // "pt":[378281,43.6042,1.44354,194,-0.00755672,-0.00907564,0.00486406,0.00774673,0.017252,0.00039099,1.76517,-0.0102706,-0.0489276,0.00582886,5.19991e-05,-0.00116022,0.00409155]}
    static Point from_pc_json( const std::string &json, const Schema &sch );

    std::vector<unsigned char> get_dim_hex( const Dimension &d ) const;

    int32_t pcid;
    std::vector<std::string> data; // not casted
    Schema schema; // data and schema.dimensions_order are well ordered
};
