#pragma once

#include        "Text.h"

class Log {
    public:
        enum {
            TRACE = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4
        };
        class Level {
            friend class Log;
            private:
                uint level;

            public:
                Level (uint _level) : level (_level) {}
                uint get () { return level; }
        };

    private:
        ostream& stream;
        uint filterLevel;
        uint level;

        Log (ostream& _stream, uint _filterLevel = INFO, uint _level = INFO) : stream (_stream), filterLevel (_filterLevel), level (_level) {
            // XXX TODO: figure out how to load these settings from a file...
        }

        static Log& singleton () {
            static Log log (cerr);
            return log;
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

        Log& operator << (const Level& _level) {
            level = _level.level;
            return *this;
        }

        Log& filter (Level& _filterLevel) {
            filterLevel = _filterLevel.level;
            return *this;
        }

        // global static accessors
        static Log& trace () { return singleton () << Level (TRACE); }
        static Log& debug () { return singleton () << Level (DEBUG); }
        static Log& info () { return singleton () << Level (INFO); }
        static Log& warn () { return singleton () << Level (WARN); }
        static Log& error () { return singleton () << Level (ERROR); }
};
