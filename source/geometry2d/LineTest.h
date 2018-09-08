#pragma once

#include "Tuple.h"

template<typename Vector>
struct Edge {
    Vector   a;
    Vector   b;
    
    Edge (void) {}
    Edge (const Vector& _a, const Vector& _b) : a (_a), b (_b) {}
};

enum LineTestResult {
    TURNS_LEFT =    1,
    TURNS_RIGHT =   -1,
    SPANNING =      TURNS_LEFT * TURNS_RIGHT,
    COLINEAR =      0
};

template<typename Scalar>
LineTest lineTestResult (Scalar lineTest, Scalar epsilon)
{
    return  (lineTest > epsilon) ? TURNS_LEFT :
            (lineTest < -epsilon) ? TURNS_RIGHT :
            COLINEAR;
}

template<typename Scalar>
Scalar RawLineTest (const TPoint2<Scalar>& a, const TPoint2<Scalar>& b, const TPoint2<Scalar>& c)
{
    return (b - a) CROSS (c - a);
}

template<typename Scalar>
sInt
LineTest (const TPoint2<Scalar>& a, const TPoint2<Scalar>& b, const TPoint2<Scalar>& c, Scalar rEpsilon = REAL_EPSILON)
{
    return LineTestResult (RawLineTest (a, b, c), rEpsilon);
}

template<typename Scalar>
sInt
LineTest (const TEdge2<Scalar>& edge, const TPoint2<Scalar>& c, Scalar rEpsilon = REAL_EPSILON)
{
    return LineTest (edge.a, edge.b, c, rEpsilon);
}

template<typename Scalar>
Scalar
NormalizedRawLineTest (const TPoint2<Scalar>& a, const TPoint2<Scalar>& b, const TPoint2<Scalar>& c)
{
    return (b - a).Normalized () CROSS (c - a).Normalized ();
}

template<typename Scalar>
sInt
NormalizedLineTest (const TPoint2<Scalar>& a, const TPoint2<Scalar>& b, const TPoint2<Scalar>& c, Scalar rEpsilon = REAL_EPSILON)
{
    return LineTestResult (NormalizedRawLineTest (a, b, c), rEpsilon);
}

template<typename Scalar>
sInt
NormalizedLineTest (const TEdge2<Scalar>& edge, const TPoint2<Scalar>& c, Scalar rEpsilon = REAL_EPSILON)
{
    return NormalizedLineTest (edge.a, edge.b, c, rEpsilon);
}
