/**
 * @file utils.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 27 Jun 2016
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <iomanip>

#include "ogr_spatialref.h"

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

bool export_to_wkt( int32_t epsg_code, std::string &wkt_str )
{
  bool rc = false;
  OGRSpatialReference oSRS;
  oSRS.importFromEPSG( epsg_code );

  char *wkt = NULL;
  if ( OGRERR_NONE == oSRS.exportToWkt( &wkt ) )
  {
    wkt_str = wkt;
    rc = true;
  }

  return rc;
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

BoundingBox BoundingBox::from_string( const std::string &str, Schema &sch )
{
  BoundingBox box;
  std::vector<std::string> bounds = split( str, ',' );

  // size=3 => [xmin, ymin, zmin]
  if ( bounds.size() == 3 )
  {
    size_t xpos = sch.get_position_x();
    size_t ypos = sch.get_position_y();
    size_t zpos = sch.get_position_z();

    box.xmin = atof(bounds[xpos].c_str());
    box.ymin = atof(bounds[ypos].c_str());
    box.zmin = atof(bounds[zpos].c_str());
    box.xmax = 0;
    box.ymax = 0;
    box.zmax = 0;
  }
  // size=6 => [xmin, ymin, zmin, xmax, ymax, zmax]
  else if ( bounds.size() == 6 )
  {
    size_t xpos = sch.get_position_x();
    size_t ypos = sch.get_position_y();
    size_t zpos = sch.get_position_z();

    box.xmin = atof(bounds[xpos].c_str());
    box.ymin = atof(bounds[ypos].c_str());
    box.zmin = atof(bounds[zpos].c_str());
    box.xmax = atof(bounds[3+xpos].c_str());
    box.ymax = atof(bounds[3+ypos].c_str());
    box.zmax = atof(bounds[3+zpos].c_str());
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
Point::Point()
  : x(0)
  , y(0)
  , z(0)
{
}

Point Point::from_string( const std::string &ptstr, const Schema &sch )
{
  Point pt;
  int32_t pos;
  std::vector<std::string> ptvals = split( ptstr, ',' );

  // get x
  pos = sch.get_position_x();
  if ( pos != -1 )
    pt.x = std::stod( ptvals[pos] );

  pos = sch.get_position_y();
  if ( pos != -1 )
    pt.y = std::stod( ptvals[pos].c_str() );

  pos = sch.get_position_z();
  if ( pos != -1 )
    pt.z = std::stod( ptvals[pos].c_str() );

  return pt;
}

bool Point::valid() const
{
  if ( x != 0 && y != 0 && z != 0 )
    return true;
  else
    return false;
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

  for (size_t j=0; j<dims.size(); j++)
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

  std::vector<std::string>::const_iterator it;
  for ( it = dimensions_order.begin(); it != dimensions_order.end(); ++it )
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

size_t Schema::get_position( const std::string &dim_name ) const
{
  size_t pos = -1;

  if ( is_defined( dim_name ) )
  {
    pos = find(dimensions_order.begin(), dimensions_order.end(),
        dim_name) - dimensions_order.begin();
  }

  return pos;
}

size_t Schema::get_position_x() const
{
  int32_t pos = get_position( "x" );
  if ( pos == -1 )
    pos = get_position( "X" );

  return pos;
}

size_t Schema::get_position_y() const
{
  int32_t pos = get_position( "y" );
  if ( pos == -1 )
    pos = get_position( "Y" );

  return pos;
}

size_t Schema::get_position_z() const
{
  int32_t pos = get_position( "z" );
  if ( pos == -1 )
    pos = get_position( "Z" );

  return pos;
}

SchemaPotreeGreyhoundInfo::SchemaPotreeGreyhoundInfo()
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
  Dimension dor ( "Classification", "unsigned", 1 );
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
}

SchemaPotreeGreyhoundRead::SchemaPotreeGreyhoundRead()
{
  // NOTE: dimension are now 'signed' with a size of '4' instead of 'floating'
  // with a size of '8' in the Info query for X/Y/Z. In fact, we have to return
  // scaled integer to Potree : scaled_int = ( val_float - offset ) / scale

  //{name:X,size:4,type:signed}
  Dimension dx ( "X", "signed", 4 );
  dimensions[ dx.name ] = dx;
  dimensions_order.push_back( dx.name );

  //{name:Y,size:4,type:signed}
  Dimension dy( "Y", "signed", 4 );
  dimensions[dy.name] = dy;
  dimensions_order.push_back(dy.name);

  //{name:Z,size:4,type:signed}
  Dimension dz( "Z", "signed", 4 );
  dimensions[dz.name] = dz;
  dimensions_order.push_back(dz.name);

  //{name:Intensity,size:2,type:unsigned}
  Dimension di ( "Intensity", "unsigned", 2 );
  dimensions[di.name] = di;
  dimensions_order.push_back( di.name );

  // classification
  Dimension dc ( "Classification", "unsigned", 1 );
  dimensions[dc.name] = dc;
  dimensions_order.push_back( dc.name );

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
}
