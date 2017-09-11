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

enum EventType {
  SELECT_EVENT_NONE         = 0,
  SELECT_EVENT_TIME         = 1,
  SELECT_EVENT_PRICE        = 2,
  SELECT_EVENT_ENROLLMENT   = 3,
  SELECT_EVENT_PRICE_INPUT  = 4,
  SELECT_EVENT_FARE         = 5,
  SELECT_EVENT_BID          = 6,
  SELECT_EVENT_OK           = 7,
  SELECT_EVENT_CANCEL       = 8,
};

}
