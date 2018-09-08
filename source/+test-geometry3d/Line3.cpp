#include "Test.h"
#include "Line3.h"

TEST_CASE(TestLine3) {
    //Log::Scope scope (Log::TRACE);
    TEST_X(true);
}

TEST_CASE(TestLine3Distance) {
    //Log::Scope scope (Log::TRACE);

    Vector3 a;
    Vector3 b (1, 1, 1);
    Vector3 c (0, 0, 1);

    auto   line = Line3<f8>::LineFromTwoPoints (a, b);
    TEST_XYEPS(line.distance (c), 0.816497, 1e-6);
}

