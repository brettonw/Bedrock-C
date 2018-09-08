#include "Test.h"
#include "Plane.h"

using namespace Geometry3d;

TEST_CASE(TestPlane) {
    Log::Scope scope (Log::TRACE);

    auto plane = Plane<f8>::PlaneFromThreePoints (Vector3 {0, 1, 0}, Vector3 {1, 1, 0}, Vector3 {0, 1, -1});
    TEST_XY (plane.getNormal (), Vector3 ({0, 1, 0}));
    TEST_XYEPS(plane.getD (), -1, 1e-9);
    TEST_XY(plane.getPlane (), Vector4 ({0, 1, 0, -1}));
    TEST_XYEPS (plane.distance (Vector3 ()), -1, 1e-9);
}
