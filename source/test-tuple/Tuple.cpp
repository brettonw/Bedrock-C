
#include "Test.h"
#include "Tuple.h"

TEST_CASE(TestTuple) {
    Log::Scope scope (Log::TRACE);

    Tuple2<double> a;
    TEST_XY (a[U], 0);
    TEST_XY (a[V], 0);
    TEST_XY (a, Tuple2<double>::ZERO);

    Tuple2<double> uv ({1.0, 2.0});
    TEST_XY (uv[U], 1);
    TEST_XY (uv[V], 2);

    Tuple3<double> abc = { 3.0, 4.0, 5.0};
    TEST_XY (abc[X], 3);
    TEST_XY (abc[Y], 4);
    TEST_XY (abc[Z], 5);
}
