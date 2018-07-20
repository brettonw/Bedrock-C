#include        "Test.h"
#include        "Log.h"
#include        "Text.h"

TEST_CASE (TestLogFilterTrace) {
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::TRACE);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    TEST_XY(Text (out.str ().c_str ()), Text("trace\ndebug\ninfo\nwarn\nerror\n"));
}

TEST_CASE (TestLogFilterDebug) {
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::DEBUG);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    TEST_XY(Text (out.str ().c_str ()), Text("debug\ninfo\nwarn\nerror\n"));
}

TEST_CASE (TestLogFilterInfo) {
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::INFO);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    TEST_XY(Text (out.str ().c_str ()), Text("info\nwarn\nerror\n"));
}

TEST_CASE (TestLogFilterWarn) {
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::WARN);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    TEST_XY(Text (out.str ().c_str ()), Text("warn\nerror\n"));
}

TEST_CASE (TestLogFilterError) {
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::ERROR);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    TEST_XY(Text (out.str ().c_str ()), Text("error\n"));
}

TEST_CASE (TestLogMulti) {
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::INFO)
        << Log::Level (Log::TRACE) << "trace"
        << Log::Level (Log::DEBUG) << "debug"
        << Log::Level (Log::INFO) << "info"
        << Log::Level (Log::WARN) << "warn"
        << Log::Level (Log::ERROR) << "error";

    TEST_XY(Text (out.str ().c_str ()), Text("infowarnerror"));
}
