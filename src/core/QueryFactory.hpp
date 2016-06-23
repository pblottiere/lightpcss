/**
 * @file QueryFactory.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 23 Jun 2016
 */

#pragma once

#include <memory>
#include <httpserver.hpp>

#include "Query.hpp"

class QueryFactory
{
  public:
    static std::unique_ptr<Query> make( const httpserver::http_request &req );
};
