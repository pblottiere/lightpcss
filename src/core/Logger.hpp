/**
 * @file Logger.hpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 28 Jun 2016
 */

#pragma once

#include <string>

class Logger
{
  public:
    static Logger& instance();

    void info( const std::string &msg ) const;
    void err( const std::string &msg ) const;

  private:
    Logger();
    ~Logger();
    void operator= (const Logger&){};
    Logger (const Logger&){};

    static Logger _instance;
};
