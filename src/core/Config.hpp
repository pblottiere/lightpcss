/**
 * @file Config.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 28 Jun 2016
 */

#pragma once

#include <string>
#include <vector>

class Config
{
  public:
    static Config& instance();

    bool valid;
    int32_t port;
    int32_t max_threads;
    std::string resource;
    std::string allow;

  private:
    Config();
    void operator= (const Config&){};
    Config (const Config&){};

    bool read();

    static Config _instance;
};
