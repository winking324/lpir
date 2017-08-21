#pragma once
#include <string>


namespace lpir {


enum LogLevel {
  TRACE_LOG_LEVEL   = 0,
  DEBUG_LOG_LEVEL   = 1,
  INFO_LOG_LEVEL    = 2,
  WARN_LOG_LEVEL    = 3,
  ERROR_LOG_LEVEL   = 4,
  FATAL_LOG_LEVEL   = 5,
  LOG_LEVEL_BUTT    = 6,
};

const static std::string LOGGER_NAME("LPIR");
const static std::string LOGGER_FORMAT("%d [%t] %-5p %c - %m%n");
const static std::string LOGGER_FILE_PATH("/tmp/lpir.log");

}
