#include "Test.h"
#include "Line.h"

using namespace Geometry2d;

TEST_CASE(TestLine) {
    //Log::Scope scope (Log::TRACE);

    Vector2 a {0, 2};
    Vector2 b {1, 1};

    auto line = Line<f8>::fromTwoPoints (a, b);
    Vector2 normal = line.getNormal ();
    Vector2 pt = line.getPoint ();

    TEST_XY(normal, (Vector2 {-1, -1}).normalized ());

    TEST_X(line.isOnLine (a));
    TEST_X(line.isOnLine (b));
    TEST_X(line.isOnLine (pt));

    TEST_XYEPS(line.distance (Vector2 ()), sqrt (2), 1e-6);
}

