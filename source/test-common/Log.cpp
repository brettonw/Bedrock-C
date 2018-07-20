#include        "Test.h"
#include        "Log.h"

TEST_CASE (TestLog) {
    Log::info() << "Hello World" << endl;
}
