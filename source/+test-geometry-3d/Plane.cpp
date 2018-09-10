#include "Test.h"
#include "Plane.h"

using namespace Geometry3d;

TEST_CASE(TestPlane) {
    Log::Scope scope (Log::TRACE);

    auto plane = Plane<f8>::fromThreePoints (Vector3 {0, 1, 0}, Vector3 {1, 1, 0}, Vector3 {0, 1, -1});
    TEST_EQUALS (plane.getNormal (), Vector3 ({0, 1, 0}));
    TEST_EQUALS_EPS(plane.getD (), -1, 1e-9);
    TEST_EQUALS(plane.getPlane (), Vector4 ({0, 1, 0, -1}));
    TEST_EQUALS_EPS (plane.distance (Vector3 ()), -1, 1e-9);
}
