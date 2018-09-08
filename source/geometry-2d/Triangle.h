#pragma once

#include "LineTest.h"

namespace Geometry2d {
    template<typename Scalar>
    class Triangle {
        private:
            const Tuple<Scalar, 2>* points;

        public:
            Triangle (const Tuple<Scalar, 2>* _points) : points (_points) {}

            Scalar area () const {
                return rawLineTest (points[0], points[1], points[2]) / 2;
            }

            bool contains (const Tuple<Scalar, 2>& point) const {
                return  (rawLineTest (points[0], points[1], point) >= 0) and
                        (rawLineTest (points[1], points[2], point) >= 0) and
                        (rawLineTest (points[2], points[0], point) >= 0);
            }
    };
}
