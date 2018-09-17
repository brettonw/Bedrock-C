#include "Test.h"
#include "Hyperplane.h"

TEST_CASE(Hyperplane2) {
    //Log::Scope scope (Log::TRACE);

    Vector2 a (0, 2);
    Vector2 b (1, 1);

    auto hyperplane = Hyperplane2::fromPoints (a, b);
    Vector2 normal = hyperplane.getNormal ();
    Vector2 pt = hyperplane.getPoint ();

    TEST_EQUALS(normal, (Vector2 (-1, -1)).normalized ());

    TEST_TRUE(hyperplane.isOn (a));
    TEST_TRUE(hyperplane.isOn (b));
    TEST_TRUE(hyperplane.isOn (pt));

    TEST_EQUALS_EPS(hyperplane.distance (Vector2 ()), sqrt (2), 1e-6);
}

/*
TEST_CASE(Hyperplane3) {
    //Log::Scope scope (Log::TRACE);

    Vector3 a;
    Vector3 b (1, 1, 1);
    Vector3 c (0, 0, 1);

    auto   line = Line<f8, 3>::fromTwoPoints (a, b);
    TEST_EQUALS_EPS(line.distance (c), 0.816497, 1e-6);
}
*/
