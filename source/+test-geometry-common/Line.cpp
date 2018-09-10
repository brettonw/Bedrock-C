#include "Test.h"
#include "Line.h"

TEST_CASE(TestLine2) {
    //Log::Scope scope (Log::TRACE);

    Vector2 a {0, 2};
    Vector2 b {1, 1};

    auto line = Line<f8, 2>::fromTwoPoints (a, b);
    //Vector2 normal = line.getNormal ();
    Vector2 pt = line.getPoint (2);

    //TEST_XY(normal, (Vector2 {-1, -1}).normalized ());

    TEST_TRUE(line.isOnLine (a));
    TEST_TRUE(line.isOnLine (b));
    TEST_TRUE(line.isOnLine (pt));

    TEST_EQUALS_EPS(line.distance (Vector2 ()), sqrt (2), 1e-6);
}

TEST_CASE(TestLine3) {
    //Log::Scope scope (Log::TRACE);

    Vector3 a;
    Vector3 b {1, 1, 1};
    Vector3 c {0, 0, 1};

    auto   line = Line<f8, 3>::fromTwoPoints (a, b);
    TEST_EQUALS_EPS(line.distance (c), 0.816497, 1e-6);
}
