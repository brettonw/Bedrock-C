#include "Test.h"
#include "Log.h"

TEST_CASE(LogFilterTrace) {
    Log::Scope scope;
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::TRACE);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    //cerr << out.str () << endl;
    TEST_EQUALS(Text (out.str ().c_str ()), Text("[TRACE] trace\n[DEBUG] debug\n[ INFO] info\n[ WARN] warn\n[ERROR] error\n"));
}

TEST_CASE(LogFilterDebug) {
    Log::Scope scope;
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::DEBUG);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    TEST_EQUALS(Text (out.str ().c_str ()), Text("[DEBUG] debug\n[ INFO] info\n[ WARN] warn\n[ERROR] error\n"));
}

TEST_CASE(LogFilterInfo) {
    Log::Scope scope;
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::INFO);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    TEST_EQUALS(Text (out.str ().c_str ()), Text("[ INFO] info\n[ WARN] warn\n[ERROR] error\n"));
}

TEST_CASE(LogFilterWarn) {
    Log::Scope scope;
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::WARN);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    TEST_EQUALS(Text (out.str ().c_str ()), Text("[ WARN] warn\n[ERROR] error\n"));
}

TEST_CASE(LogFilterError) {
    Log::Scope scope;
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::ERROR);

    Log::trace() << "trace" << endl;
    Log::debug() << "debug" << endl;
    Log::info() << "info" << endl;
    Log::warn() << "warn" << endl;
    Log::error() << "error" << endl;

    TEST_EQUALS(Text (out.str ().c_str ()), Text("[ERROR] error\n"));
}

TEST_CASE(LogMulti) {
    Log::Scope scope;
    ostringstream out;

    Log::base().setStream (out).setFilterLevel (Log::INFO)
        << Log::Level (Log::TRACE) << "trace"
        << Log::Level (Log::DEBUG) << "debug"
        << Log::Level (Log::INFO) << "info"
        << Log::Level (Log::WARN) << "warn"
        << Log::Level (Log::ERROR) << "error";

    TEST_EQUALS(Text (out.str ().c_str ()), Text("infowarnerror"));
}

TEST_CASE(LogScope) {
    Log::Scope scope(Log::INFO);

    ostringstream out;
    {
        Log::Scope scope2;
        Log::base().setStream (out).setFilterLevel (Log::INFO);
        Log::trace() << "trace" << endl;
        Log::debug() << "debug" << endl;
        Log::info() << "info" << endl;
        Log::warn() << "warn" << endl;
        Log::error() << "error" << endl;
    }
    TEST_EQUALS(Text (out.str ().c_str ()), Text("[ INFO] info\n[ WARN] warn\n[ERROR] error\n"));

    {
        Log::Scope scope2 (Log::DEBUG);
        Log::base().setStream (out);
        Log::trace() << "trace" << endl;
        Log::debug() << "debug" << endl;
        Log::info() << "info" << endl;
        Log::warn() << "warn" << endl;
        Log::error() << "error" << endl;
    }
    TEST_EQUALS(Text (out.str ().c_str ()), Text("[ INFO] info\n[ WARN] warn\n[ERROR] error\n[DEBUG] debug\n[ INFO] info\n[ WARN] warn\n[ERROR] error\n"));

    {
        Log::Scope scope2;
        Log::base().setStream (out);
        Log::trace() << "trace" << endl;
        Log::debug() << "debug" << endl;
        Log::info() << "info" << endl;
        Log::warn() << "warn" << endl;
        Log::error() << "error" << endl;
    }
    TEST_EQUALS(Text (out.str ().c_str ()), Text("[ INFO] info\n[ WARN] warn\n[ERROR] error\n[DEBUG] debug\n[ INFO] info\n[ WARN] warn\n[ERROR] error\n[ INFO] info\n[ WARN] warn\n[ERROR] error\n"));
}
