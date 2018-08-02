#include "Test.h"

// some checks to see what the memory/architecture model is
static_assert((sizeof(short) == 2) and (sizeof(int) == 4) and (sizeof(long) == 8), "unsupported data model");

TEST_CASE(TestTypeSizes) {
    // these tests should pass if compilation succeeded
    TEST_XY(sizeof (u1), 1);
    TEST_XY(sizeof (u2), 2);
    TEST_XY(sizeof (u4), 4);
    TEST_XY(sizeof (u8), 8);

    TEST_XY(sizeof (f4), 4);
    TEST_XY(sizeof (f8), 8);
}

