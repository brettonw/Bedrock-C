
#include "Test.h"
#include "Tuple.h"

TEST_CASE(TestTuple) {
    //Log::Scope scope (Log::TRACE);

    Tuple2<double> a;
    TEST_XY (a[U], 0);
    TEST_XY (a[V], 0);
    TEST_XY (a, Tuple2<double>::ZERO);
    TEST_XY (a.normL1(), 0);
    TEST_XY (a.normL2(), 0);
    TEST_XY (a.normInf(), 0);
    TEST_XY (a.normPower(1), 0);
    TEST_XY (a.normPower(2), 0);
    TEST_XY (a.normPower(10), 0);

    TEST_XY (a.norm(), a.normL2());
    TEST_XY (a.length(), a.normL2());

    Tuple2<double> uv (1.0, 2.0);
    TEST_XY (uv[U], 1);
    TEST_XY (uv[V], 2);
    TEST_XYOP (uv, Tuple2<double>::ZERO, !=);

    Tuple3<double> abc = {3.0, 4.0, 5.0};
    TEST_XY (abc[X], 3);
    TEST_XY (abc[Y], 4);
    TEST_XY (abc[Z], 5);

    Tuple3<double> def (6.0, 7.0, 8.0);
    TEST_XY (def[X], 6);
    TEST_XY (def[Y], 7);
    TEST_XY (def[Z], 8);

    Tuple3<double> xyz = def - abc;
    TEST_XY (xyz, Tuple3<double> (3, 3, 3));
    TEST_XY (xyz[X], 3);
    TEST_XY (xyz[Y], 3);
    TEST_XY (xyz[Z], 3);
}

TEST_CASE(TestTupleNorm) {
    //Log::Scope scope (Log::TRACE);

    Tuple2<double> a (3.0, 4.0);
    TEST_XY (a[U], 3);
    TEST_XY (a[V], 4);
    TEST_XYOP (a, Tuple2<double>::ZERO, !=);

    TEST_XY (a.normL1(), 7);
    TEST_XY (a.normL2(), 5);
    TEST_XY (a.normInf(), 4);
    TEST_XY (a.normPower(1), 7);
    TEST_XY (a.normPower(2), 5);

    TEST_XY (a.norm(), a.normL2());
    TEST_XY (a.length(), a.normL2());
}

TEST_CASE(TestTupleUnit) {
    //Log::Scope scope (Log::TRACE);

    Tuple2<double> a = (Tuple2<double> (2.0, 0.0)).normalized();
    TEST_XY(a.norm (), 1); TEST_XY(a[X], 1); TEST_XY(a[Y], 0);

    Tuple2<double> b = (Tuple2<double> (0.0, 2.0)).normalized();
    TEST_XY(b.norm (), 1); TEST_XY(b[X], 0); TEST_XY(b[Y], 1);

    Tuple2<double> c = (Tuple2<double> (-2.0, 0.0)).normalized();
    TEST_XY(c.norm (), 1); TEST_XY(c[X], -1); TEST_XY(c[Y], 0);

    Tuple2<double> d = (Tuple2<double> (0.0, -2.0)).normalized();
    TEST_XY(d.norm (), 1); TEST_XY(d[X], 0); TEST_XY(d[Y], -1);

    Tuple2<double> e = (Tuple2<double> (2.0, 2.0)).normalized();
    TEST_XYF(e.norm (), 1, 1.0e-9); TEST_XY(e[X], e[Y]);

    Tuple2<double> f = (Tuple2<double> (-2.0, 2.0)).normalized();
    TEST_XYF(f.norm (), 1, 1.0e-9); TEST_XY(-f[X], f[Y]);

    Tuple2<double> g = (Tuple2<double> (-2.0, -2.0)).normalized();
    TEST_XYF(g.norm (), 1, 1.0e-9); TEST_XY(g[X], g[Y]);

    Tuple2<double> h = (Tuple2<double> (2.0, -2.0)).normalized();
    TEST_XYF(h.norm (), 1, 1.0e-9); TEST_XY(h[X], -h[Y]);
}

TEST_CASE(TestTupleOps) {
    Log::Scope scope (Log::TRACE);

    TEST_XY (Tuple2<double>(1.0, 0.0) * 4, Tuple2<double> (4, 0));
    TEST_XY (Tuple2<double>(0.0, 1.0) * 4, Tuple2<double> (0, 4));
    TEST_XY (Tuple2<double>(1.0, 1.0) * 4, Tuple2<double> (4, 4));

    TEST_XY (4 * Tuple2<double>(1.0, 0.0), Tuple2<double> (4, 0));
    TEST_XY (4 * Tuple2<double>(0.0, 1.0), Tuple2<double> (0, 4));
    TEST_XY (4 * Tuple2<double>(1.0, 1.0), Tuple2<double> (4, 4));

    TEST_XY (Tuple2<double>(4.0, 0.0) / 4, Tuple2<double> (1, 0));
    TEST_XY (Tuple2<double>(0.0, 4.0) / 4, Tuple2<double> (0, 1));
    TEST_XY (Tuple2<double>(4.0, 4.0) / 4, Tuple2<double> (1, 1));

    TEST_XY (Tuple2<double>(1.0, 0.0) + Tuple2<double>(1.0, 0.0), Tuple2<double> (2, 0));
    TEST_XY (Tuple2<double>(1.0, 0.0) - Tuple2<double>(1.0, 0.0), Tuple2<double> (0, 0));
}


