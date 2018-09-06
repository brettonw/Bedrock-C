
#include "Test.h"
#include "Tuple.h"

TEST_CASE(TestTuple) {
    //Log::Scope scope (Log::TRACE);

    Vector2 a;
    TEST_XY (a[U], 0);
    TEST_XY (a[V], 0);
    TEST_XY (a, Vector2::ZERO);
    TEST_XY (a.normL1(), 0);
    TEST_XY (a.normL2(), 0);
    TEST_XY (a.normInf(), 0);
    TEST_XY (a.normPower(1), 0);
    TEST_XY (a.normPower(2), 0);
    TEST_XY (a.normPower(10), 0);

    TEST_XY (a.norm(), a.normL2());
    TEST_XY (a.length(), a.normL2());

    Vector2 uv (1.0, 2.0);
    TEST_XY (uv[U], 1);
    TEST_XY (uv[V], 2);
    TEST_XYOP (uv, Vector2::ZERO, !=);

    Vector3 abc = {3.0, 4.0, 5.0};
    TEST_XY (abc[X], 3);
    TEST_XY (abc[Y], 4);
    TEST_XY (abc[Z], 5);

    Vector3 def (6, 7, 8);
    TEST_XY (def[X], 6);
    TEST_XY (def[Y], 7);
    TEST_XY (def[Z], 8);

    Vector3 xyz = def - abc;
    TEST_XY (xyz, Vector3 (3, 3, 3));
    TEST_XY (xyz[X], 3);
    TEST_XY (xyz[Y], 3);
    TEST_XY (xyz[Z], 3);
}

TEST_CASE(TestTupleConstructors) {
    Log::Scope scope (Log::TRACE);
    // basic empty constructor
    Vector3 a;
    TEST_XY(a, Vector3 (0, 0, 0));

    // fill constructor
    Vector3 b (1);
    TEST_XY(b, Vector3 (1, 1, 1));

    // copy constructor
    Vector3 c (b);
    TEST_XY(c, Vector3 (1, 1, 1));

    // from an array of doubles
    double xyz[] = {2, 2, 2};
    Vector3 d (xyz);
    TEST_XY(d, Vector3 (2, 2, 2));

    // from a bigger tuple
    Vector2 e (d);
    TEST_XY(e, Vector2 (2, 2));

    // from a smaller tuple with fill
    Vector3 f (e, 5);
    TEST_XY(f, Vector3 (2, 2, 5));

    // and the explicitly specialized constructor from the right number of scalars
    Vector3 g (2, 3, 4);
    TEST_XY(g, Vector3 (2, 3, 4));
}

TEST_CASE(TestTupleEquality) {
    Log::Scope scope (Log::TRACE);
    TEST_X (true);
}

TEST_CASE(TestTupleNorm) {
    //Log::Scope scope (Log::TRACE);

    Vector2 a (3.0, 4.0);
    TEST_XY (a[U], 3);
    TEST_XY (a[V], 4);
    TEST_XYOP (a, Vector2::ZERO, !=);

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

    Vector2 a = (Vector2 (2.0, 0.0)).normalized();
    TEST_XY(a.norm (), 1); TEST_XY(a[X], 1); TEST_XY(a[Y], 0); TEST_X(a.isUnit());

    Vector2 b = (Vector2 (0.0, 2.0)).normalized();
    TEST_XY(b.norm (), 1); TEST_XY(b[X], 0); TEST_XY(b[Y], 1); TEST_X(b.isUnit());

    Vector2 c = (Vector2 (-2.0, 0.0)).normalized();
    TEST_XY(c.norm (), 1); TEST_XY(c[X], -1); TEST_XY(c[Y], 0); TEST_X(c.isUnit());

    Vector2 d = (Vector2 (0.0, -2.0)).normalized();
    TEST_XY(d.norm (), 1); TEST_XY(d[X], 0); TEST_XY(d[Y], -1); TEST_X(d.isUnit());

    Vector2 e = (Vector2 (2.0, 2.0)).normalized();
    TEST_XYF(e.norm (), 1, 1.0e-9); TEST_XY(e[X], e[Y]); TEST_X(e.isUnit());

    Vector2 f = (Vector2 (-2.0, 2.0)).normalized();
    TEST_XYF(f.norm (), 1, 1.0e-9); TEST_XY(-f[X], f[Y]); TEST_X(f.isUnit());

    Vector2 g = (Vector2 (-2.0, -2.0)).normalized();
    TEST_XYF(g.norm (), 1, 1.0e-9); TEST_XY(g[X], g[Y]); TEST_X(g.isUnit());

    Vector2 h = (Vector2 (2.0, -2.0)).normalized();
    TEST_XYF(h.norm (), 1, 1.0e-9); TEST_XY(h[X], -h[Y]); TEST_X(h.isUnit());
}

TEST_CASE(TestTupleOps) {
    //Log::Scope scope (Log::TRACE);

    TEST_XY (-Vector2(1.0, 0.0), Vector2 (-1, 0));

    TEST_XY (Vector2(1.0, 0.0) * 4, Vector2 (4, 0));
    TEST_XY (Vector2(0.0, 1.0) * 4, Vector2 (0, 4));
    TEST_XY (Vector2(1.0, 1.0) * 4, Vector2 (4, 4));

    TEST_XY (4 * Vector2(1.0, 0.0), Vector2 (4, 0));
    TEST_XY (4 * Vector2(0.0, 1.0), Vector2 (0, 4));
    TEST_XY (4 * Vector2(1.0, 1.0), Vector2 (4, 4));

    TEST_XY (Vector2(4.0, 0.0) / 4, Vector2 (1, 0));
    TEST_XY (Vector2(0.0, 4.0) / 4, Vector2 (0, 1));
    TEST_XY (Vector2(4.0, 4.0) / 4, Vector2 (1, 1));

    TEST_XY (Vector2(1.0, 0.0) + Vector2(1.0, 0.0), Vector2 (2, 0));
    TEST_XY (Vector2(1.0, 0.0) - Vector2(1.0, 0.0), Vector2 (0, 0));

    TEST_XY (Vector2(1.0, 0.0) DOT Vector2(1.0, 0.0), 1);
    TEST_XY (Vector2(1, 0) CROSS Vector2 (0, 1), 1);
}


