/**
 * @file Config.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 28 Jun 2016
 */

#include <fstream>
#include <sstream>
#include <iostream>

#include "Config.hpp"

Config Config::_instance=Config();

Config::Config()
  : valid( false )
  , logfile("")
  , port( 8080 )
  , max_threads( 5 )
  , resource( "/resource" )
  , allow( "" )
{
  read();
}

Config& Config::instance()
{
  return _instance;
}

bool Config::read()
{
  bool rc = false;

  // read etc file in /etc/lightpcss.conf
  // or /usr/local/etc/lightpcss.conf
  std::string conf_root = "/etc/lightpcss.conf";
  std::string conf_local = "/usr/local/etc/lightpcss.conf";
  std::string current_conf = "";

  if ( std::ifstream( conf_root ) )
    current_conf = conf_root;
  else if ( std::ifstream( conf_local ) )
    current_conf = conf_local;


  if ( ! current_conf.empty() )
  {
    std::ifstream if_file(current_conf);
    std::string conf_str((std::istreambuf_iterator<char>(if_file)),
      std::istreambuf_iterator<char>());

    std::istringstream is_file( conf_str );
    std::string line;

    while( std::getline( is_file, line ) )
    {
      std::istringstream is_line( line );
      std::string key;
      if( std::getline( is_line, key, '=' ) )
      {
        std::string value;

        if( std::getline( is_line, value ) )
        {
          if ( key == "logfile" )
            logfile = value;
          else if ( key == "port" )
            port = atoi( value.c_str() );
          else if ( key == "max_threads" )
            max_threads = atoi( value.c_str() );
          else if ( key == "resource" )
            resource = value;
          else if ( key == "allow_access_control" )
            allow = value;
        }
      }
    }

    rc = true;
  }

  return rc;
}
