#pragma once

#include "Tuple.h"

namespace Geometry2d {
    template<typename Scalar>
    struct Edge {
        Tuple<Scalar, 2> a;
        Tuple<Scalar, 2> b;

        Edge (void) {}
        Edge (const Tuple<Scalar, 2>& _a, const Tuple<Scalar, 2>& _b) : a (_a), b (_b) {}
    };

    // line tests return the area of the parallelogram subtended by the three points, which has a lot of
    // applications in the computational geometry. a positive value indicates a left turning sequence,
    // and a negative value indicates a right turning sequence.

    enum LineTest {
        TURNS_LEFT =    1,
        TURNS_RIGHT =   -1,
        SPANNING =      TURNS_LEFT * TURNS_RIGHT,
        COLINEAR =      0
    };

    template<typename Scalar>
    LineTest classifyLineTestResult (Scalar lineTest) {
        return  (lineTest > Tuple<Scalar, 2>::epsilon) ? TURNS_LEFT :
                (lineTest < -Tuple<Scalar, 2>::epsilon) ? TURNS_RIGHT :
                COLINEAR;
    }

    template<typename Scalar>
    Scalar rawLineTest (const Tuple<Scalar, 2>& a, const Tuple<Scalar, 2>& b, const Tuple<Scalar, 2>& c) {
        return (b - a) CROSS (c - a);
    }

    template<typename Scalar>
    LineTest lineTest (const Tuple<Scalar, 2>& a, const Tuple<Scalar, 2>& b, const Tuple<Scalar, 2>& c) {
        return classifyLineTestResult (rawLineTest (a, b, c));
    }

    template<typename Scalar>
    LineTest lineTest (const Edge<Scalar>& edge, const Tuple<Scalar, 2>& c) {
        return lineTest (edge.a, edge.b, c);
    }

    template<typename Scalar>
    Scalar normalizedRawLineTest (const Tuple<Scalar, 2>& a, const Tuple<Scalar, 2>& b, const Tuple<Scalar, 2>& c) {
        return (b - a).Normalized () CROSS (c - a).Normalized ();
    }

    template<typename Scalar>
    LineTest normalizedLineTest (const Tuple<Scalar, 2>& a, const Tuple<Scalar, 2>& b, const Tuple<Scalar, 2>& c) {
        return classifyLineTestResult (normalizedRawLineTest (a, b, c));
    }

    template<typename Scalar>
    LineTest normalizedLineTest (const Edge<Scalar>& edge, const Tuple<Scalar, 2>& c) {
        return normalizedLineTest (edge.a, edge.b, c);
    }
}
