/**
 * @file Logger.cpp
 * @author Paul Blottiere paul.blottiere@oslandia.com
 * @date 28 Jun 2016
 */

#include <syslog.h>
#include <unistd.h>

#include "Logger.hpp"

Logger Logger::_instance=Logger();

Logger::Logger()
{
  openlog("lightpcss", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL0 );
}

Logger::~Logger()
{
  closelog();
}

Logger& Logger::instance()
{
  return _instance;
}

void Logger::info( const std::string &msg ) const
{
  syslog( LOG_INFO, msg.c_str(), getuid() );
}

void Logger::err( const std::string &msg ) const
{
  syslog( LOG_ERR, msg.c_str(), getuid() );
}
