#include        "Log.h"


Log& LOG () {
    // setup the log levels
    LogLevel TRACE (0); LogLevel::TRACE = TRACE;
    LogLevel DEBUG (1); LogLevel::DEBUG = DEBUG;
    LogLevel INFO (2); LogLevel::INFO = INFO;
    LogLevel WARN (3); LogLevel::WARN = WARN;
    LogLevel ERROR (4); LogLevel::ERROR = ERROR;

    // XXX TODO: figure out how to load the settings from a file...
    static Log log (cerr, LogLevel::INFO);
    return log;
}
