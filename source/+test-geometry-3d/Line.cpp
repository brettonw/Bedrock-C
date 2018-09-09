#include "Test.h"
#include "Line.h"

using namespace Geometry3d;

TEST_CASE(TestLineDistance) {
    //Log::Scope scope (Log::TRACE);

    Vector3 a;
    Vector3 b {1, 1, 1};
    Vector3 c {0, 0, 1};

    auto   line = Line<f8>::LineFromTwoPoints (a, b);
    TEST_XYEPS(line.distance (c), 0.816497, 1e-6);
}

