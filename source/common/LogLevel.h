#pragma once

#include        "Types.h"

class LogLevel {
    private:
        uint level;

    public:
        LogLevel () : level (-1) {}
        LogLevel (uint _level) : level (_level) {}
        LogLevel& operator = (LogLevel& _level) { level = _level.level; return *this; }
        LogLevel (LogLevel& _level) : level (_level.level) {}
        uint operator () () { return level; }
        bool operator >= (LogLevel& _level) { return level >= _level.level; }

        static LogLevel TRACE;
        static LogLevel DEBUG;
        static LogLevel INFO;
        static LogLevel WARN;
        static LogLevel ERROR;
};

