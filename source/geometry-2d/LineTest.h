#pragma once

#include "Edge.h"

namespace Geometry2d {
    // line tests return the area of the parallelogram subtended by the three points, which has a lot of
    // applications in the computational geometry. a positive value indicates a left turning sequence,
    // and a negative value indicates a right turning sequence.

    enum LineClass {
        TURNS_LEFT =    1,
        TURNS_RIGHT =   -1,
        SPANNING =      TURNS_LEFT * TURNS_RIGHT,
        COLINEAR =      0
    };

    template<typename Scalar>
    class LineTest {
        public:
            static LineClass classify (Scalar lineTest) {
                Scalar epsilon = Tuple<Scalar, 2>::getEpsilon();
                return  (lineTest > epsilon) ? LineClass::TURNS_LEFT :
                        (lineTest < -epsilon) ? LineClass::TURNS_RIGHT :
                        LineClass::COLINEAR;
            }

            static Scalar raw (const Tuple<Scalar, 2>& a, const Tuple<Scalar, 2>& b, const Tuple<Scalar, 2>& c) {
                return (b - a) CROSS (c - a);
            }

            static LineClass classified (const Tuple<Scalar, 2>& a, const Tuple<Scalar, 2>& b, const Tuple<Scalar, 2>& c) {
                return classify (raw (a, b, c));
            }

            static LineClass classified (const Edge<Scalar, 2>& edge, const Tuple<Scalar, 2>& c) {
                return classified (edge.a, edge.b, c);
            }

            static Scalar normalizedRaw (const Tuple<Scalar, 2>& a, const Tuple<Scalar, 2>& b, const Tuple<Scalar, 2>& c) {
                return (b - a).normalized () CROSS (c - a).normalized ();
            }
    };
}
