/**
 * @file Resource.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#pragma once

#include <httpserver.hpp>

class Resource : public httpserver::http_resource
{
  public:
    void render( const httpserver::http_request&, httpserver::http_response** );
};
