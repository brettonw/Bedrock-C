#include "Test.h"
#include "LineTest.h"

using namespace Geometry2d;

TEST_CASE(LineTest) {
    //Log::Scope scope (Log::TRACE);

    Vector2 a {0, 2};
    Vector2 b {2, 0};
    Vector2 c {3, 0};
    Vector2 d {2, -1};
    Vector2 e {4, -2};

    TEST_EQUALS (LineTest<f8>::classified (a, b, c), LineClass::TURNS_LEFT);
    TEST_EQUALS (LineTest<f8>::classified (a, b, d), LineClass::TURNS_RIGHT);
    TEST_EQUALS (LineTest<f8>::classified (a, b, e), LineClass::COLINEAR);

    Edge<f8, 2> edge (a, b);
    TEST_EQUALS (LineTest<f8>::classified (edge, c), LineClass::TURNS_LEFT);
    TEST_EQUALS (LineTest<f8>::classified (edge, d), LineClass::TURNS_RIGHT);
    TEST_EQUALS (LineTest<f8>::classified (edge, e), LineClass::COLINEAR);
}

