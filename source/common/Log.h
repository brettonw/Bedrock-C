#pragma once

#include        "LogLevel.h"

class Log {
    friend Log& LOG ();

    private:
        ostream& stream;
        LogLevel filterLevel;
        LogLevel level;

        Log (ostream& _stream, LogLevel _filterLevel = LogLevel::INFO, LogLevel _level = LogLevel::INFO) : stream (_stream), filterLevel (_filterLevel), level (_level) {
        }

    public:
        template <typename InputType>
        Log& operator << (const InputType& input) {
            if (level >= filterLevel) {
                stream << input;
            }
            return *this;
        }

        Log& operator << (std::ostream&(*f)(std::ostream&)) {
            if (level >= filterLevel) {
                stream << f;
            }
            return *this;
        }

        Log& operator << (LogLevel& _level) {
            level = _level;
            return *this;
        }

        Log& filter (LogLevel& _filterLevel) {
            filterLevel = _filterLevel;
            return *this;
        }
};

// the global static accessor
Log& LOG ();
