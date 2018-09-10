#pragma once

#include "LineTest.h"

namespace Geometry2d {
    template<typename Scalar>
    class Triangle {
        private:
            const Tuple<Scalar, 2> a;
            const Tuple<Scalar, 2> b;
            const Tuple<Scalar, 2> c;

        public:
            Triangle (const Tuple<Scalar, 2>* points) : a (points[0]), b (points[1]), c (points[2]) {}
            Triangle (const Tuple<Scalar, 2>* points, uint _a, uint _b, uint _c) : a (points[_a]), b (points[_b]), c (points[_c]) {}
            Triangle (const Tuple<Scalar, 2>* points, uint* index) : a (points[index[0]]), b (points[index[1]]), c (points[index[2]]) {}

            Scalar area () const {
                return LineTest<Scalar>::raw (a, b, c) / 2;
            }

            bool contains (const Tuple<Scalar, 2>& point, LineClass lineTest) const {
                return  (LineTest<Scalar>::classified (a, b, point) == lineTest) and
                        (LineTest<Scalar>::classified (b, c, point) == lineTest) and
                        (LineTest<Scalar>::classified (c, a, point) == lineTest);
            }
    };
}
