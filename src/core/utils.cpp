/**
 * @file utils.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 27 Jun 2016
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "utils.hpp"

// ----------------------------------------------------------------------------
// Functions
// ----------------------------------------------------------------------------
std::vector<std::string> split(const std::string &s, char delim)
{
  std::string item;
  std::vector<std::string> tokens;
  std::string scopy = s;

  if ( scopy[0] == '[' )
    scopy.erase( 0, 1 );

  if ( scopy[ scopy.size() - 1 ] == ']' )
    scopy.erase( scopy.size() - 1, 1 );

  std::stringstream ss(scopy);
  while (getline(ss, item, delim))
    tokens.push_back(item);

  return tokens;
}

// ----------------------------------------------------------------------------
// Bounding box
// ----------------------------------------------------------------------------
BoundingBox::BoundingBox()
  : xmin( -1 )
  , xmax( -1 )
  , ymin( -1 )
  , ymax( -1 )
  , zmin( -1 )
  , zmax( -1 )
{
}

BoundingBox::BoundingBox( float xmin, float xmax, float ymin, float ymax,
   float zmin, float zmax )
  : xmin( xmin )
  , xmax( xmax )
  , ymin( ymin )
  , ymax( ymax )
  , zmin( zmin )
  , zmax( zmax )
{
}

BoundingBox::BoundingBox( const std::string &xmin, const std::string &xmax,
    const std::string &ymin, const std::string &ymax,
    const std::string &zmin, const std::string &zmax )
  : BoundingBox( atof( xmin.c_str() ), atof( xmax.c_str() ),
      atof( ymin.c_str() ), atof( ymax.c_str() ),
      atof( zmin.c_str() ), atof( zmax.c_str() ) )
{
}

std::string BoundingBox::to_string() const
{
  std::string boxstr;

  boxstr = xmin_str() + " " + ymin_str()
    + ", " + xmax_str() + " " + ymin_str()
    + ", " + xmax_str() + " " + ymax_str()
    + ", " + xmin_str() + " " + ymax_str()
    + ", " + xmin_str() + " " + ymin_str();

  return boxstr;
}

bool BoundingBox::is_valid() const
{
  return (xmin != -1) && (xmax != -1) && (ymin != -1)
    && (ymax != -1) && (zmin != -1) && (zmax != -1);
}

BoundingBox BoundingBox::from_string( const std::string &str )
{
  BoundingBox box;

  // [int, int, int, ...]
  bool box_ok = false;
  std::vector<std::string> bounds = split( str, ',' );

  if ( bounds.size() == 6 )
  {
    box.xmin = atof(bounds[0].c_str());
    box.ymin = atof(bounds[1].c_str());
    box.zmin = atof(bounds[2].c_str());
    box.xmax = atof(bounds[3].c_str());
    box.ymax = atof(bounds[4].c_str());
    box.zmax = atof(bounds[5].c_str());
  }

  return box;
}

std::string BoundingBox::xmin_str() const
{
  return std::to_string(xmin);
}

std::string BoundingBox::xmax_str() const
{
  return std::to_string(xmax);
}

std::string BoundingBox::ymin_str() const
{
  return std::to_string(ymin);
}

std::string BoundingBox::ymax_str() const
{
  return std::to_string(ymax);
}

std::string BoundingBox::zmin_str() const
{
  return std::to_string(zmin);
}

std::string BoundingBox::zmax_str() const
{
  return std::to_string(zmax);
}

Json::Value BoundingBox::json_value() const
{
  Json::Value v;
  v.append( xmin );
  v.append( ymin );
  v.append( zmin );
  v.append( xmax );
  v.append( ymax );
  v.append( zmax );

  return v;;
}

// ----------------------------------------------------------------------------
// Point
// ----------------------------------------------------------------------------
Point Point::from_pc_json( const std::string &json, const Schema &sch )
{
  Point pt;
  Json::Value root;
  Json::Reader reader;

  if ( reader.parse( json, root ) )
  {
    pt.pcid = root.get("pcid", "").asInt();

    Json::Value array = root.get("pt", "");
    Json::FastWriter fastWriter;
    std::string data_str = fastWriter.write(array);
    pt.data = split( data_str, ',' );
  }

  return pt;
}

std::vector<unsigned char> Point::get_dim_hex( const Dimension &d ) const
{
  std::vector<unsigned char> bytes;
  size_t pos = find(schema.dimensions_order.begin(),
      schema.dimensions_order.end(), d.name) - schema.dimensions_order.begin();

  if ( pos != schema.dimensions_order.size() )
  {
    std::string datastr = data[pos];
    std::cout << "datastr for: " << d.name << " is " << datastr << std::endl;
    std::cout << "dim size: " << d.size << std::endl;

    float f = atof( datastr.c_str() );
    std::cout << f << std::endl;
    unsigned char *ptr = (unsigned char *)&f;

    std::cout << "size: " << bytes.size() << std::endl;

    for (int i=0; i<sizeof(int32_t); i++)
      printf("%02x ", ptr[i]);

    for ( int j=0; j<bytes.size(); j++ )
      printf("%02x ", bytes[j]);
    std::cout << std::endl;
  }

  return bytes;
}

// ----------------------------------------------------------------------------
// Dimension
// ----------------------------------------------------------------------------
Dimension::Dimension()
  : name( "" )
  , type( "" )
  , size( -1 )
{
}

Dimension::Dimension( const std::string &name, const std::string type, size_t size )
  : name( name )
  , type( type )
  , size( size )
{
}

void Dimension::from_pc_json( const std::string &json, Dimension &dim )
{
  Json::Value root_dim;
  Json::Reader reader_dim;
  if ( reader_dim.parse( json, root_dim ) )
  {
    dim.name = root_dim.get("name", "").asString();
    dim.size = root_dim.get("size", "").asInt();
    dim.type = root_dim.get("type", "").asString();
  }
}

static Dimension from_potree_json( const std::string &json )
{
  Dimension dim;

  return dim;
}

Dimension Dimension::to_potree_dimension( const Dimension &pc_dim )
{
  Dimension potree_dim = pc_dim;
  potree_dim.name = potree_type( pc_dim.name );
  return potree_dim;
}

std::string Dimension::potree_type( const std::string &pc_type )
{
  std::string potree_type = pc_type;

  if ( pc_type == "double" )
    potree_type = "floating";
  else if ( pc_type == "int32_t" )
    potree_type = "signed";

  return potree_type;
}

Json::Value Dimension::json_value() const
{
  Json::Value v;
  v["name"] = name;
  v["size"] = size;
  v["type"] = potree_type(type);
  return v;
}

// ----------------------------------------------------------------------------
// Schema
// ----------------------------------------------------------------------------
Schema Schema::from_json( const std::string &json )
{
  Schema sch;

  std::vector<std::string> dims = split( json, '}' );

  for (int j=0; j<dims.size(); j++)
  {
    std::string dimstr = dims[j] + '}';
    if ( dimstr[0] == ',' )
      dimstr.erase( 0, 1 );

    Dimension d;
    Dimension::from_pc_json( dimstr, d );
    sch.dimensions[d.name] = d;

    sch.dimensions_order.push_back( d.name );
  }

  return sch;
}

Json::Value Schema::json_value() const
{
  Json::Value v;
  Json::FastWriter writer;

  std::vector<std::string>::const_iterator it = dimensions_order.begin();
  for ( it; it != dimensions_order.end(); ++it )
  {
    Dimension dim = dimensions.at(*it);
    v.append( dim.json_value() );
  }

  return v;
}

bool Schema::is_defined( const std::string &dim ) const
{
  return dimensions.find( dim ) != dimensions.end() ;
}

SchemaPotreeGreyhound::SchemaPotreeGreyhound()
{
  // {"name":"X","size":8,"type":"floating"},
  Dimension dx( "X", "floating", 8 );
  dimensions[dx.name] = dx;
  dimensions_order.push_back(dx.name);

  //{"name":"Y","size":8,"type":"floating"},
  Dimension dy( "Y", "floating", 8 );
  dimensions[dy.name] = dy;
  dimensions_order.push_back(dy.name);

  //{"name":"Z","size":8,"type":"floating"},
  Dimension dz( "Z", "floating", 8 );
  dimensions[dz.name] = dz;
  dimensions_order.push_back(dz.name);

  //{"name":"Intensity","size":2,"type":"unsigned"},
  Dimension di ( "Intensity", "unsigned", 2 );
  dimensions[di.name] = di;
  dimensions_order.push_back( di.name );

  //{"name":"Origin","size":4,"type":"unsigned"}
  Dimension dor ( "Origin", "unsigned", 4 );
  dimensions[dor.name] = dor;
  dimensions_order.push_back( dor.name );

  //{"name":"Red","size":2,"type":"unsigned"},
  Dimension dred ( "Red", "unsigned", 2 );
  dimensions[dred.name] = dred;
  dimensions_order.push_back( dred.name );

  //{"name":"Green","size":2,"type":"unsigned"},
  Dimension dg ( "Green", "unsigned", 2 );
  dimensions[dg.name] = dg;
  dimensions_order.push_back( dg.name );

  //{"name":"Blue","size":2,"type":"unsigned"},
  Dimension db ( "Blue", "unsigned", 2 );
  dimensions[db.name] = db;
  dimensions_order.push_back( db.name );

  //{"name":"ReturnNumber","size":1,"type":"unsigned"},
  //{"name":"NumberOfReturns","size":1,"type":"unsigned"},
  //{"name":"ScanDirectionFlag","size":1,"type":"unsigned"},
  //{"name":"EdgeOfFlightLine","size":1,"type":"unsigned"},
  //{"name":"Classification","size":1,"type":"unsigned"},
  //{"name":"ScanAngleRank","size":4,"type":"floating"},
  //{"name":"UserData","size":1,"type":"unsigned"},
  //{"name":"PointSourceId","size":2,"type":"unsigned"},
}
