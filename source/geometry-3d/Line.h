#pragma once

#include "Tuple.h"

namespace Geometry3d {
    template<typename Scalar>
    class Line {
        private:
            Tuple<Scalar, 3> origin;
            Tuple<Scalar, 3> direction;

        public:
            Line (const Tuple<Scalar, 3>& _origin, const Tuple<Scalar, 3>& _direction) : origin (_origin), direction (_direction) {}

            static Line<Scalar> LineFromTwoPoints (const Tuple<Scalar, 3>& a, const Tuple<Scalar, 3>& b) {
                return Line<Scalar> (a, (b - a).normalized ());
            }

            // return the shortest scalar distance from a point to the line (along a perpendicular vector)
            Scalar distance (const Tuple<Scalar, 3>& point) const {
                // the wolfram method - http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
                return sqrt ((direction CROSS (origin - point)).normL2Sq () / direction.normL2Sq ());
            }

            bool isOnLine (const Tuple<Scalar, 3>& point) const {
                return distance (point) <= Tuple<Scalar, 3>::getEpsilon ();
            }

            Tuple<Scalar, 3>& getOrigin () const {
                return origin;
            }

            Tuple<Scalar, 3>& getDirection () const {
                return direction;
            }

            Tuple<Scalar, 3> getPoint (Scalar t) {
                return origin + (direction * t);
            }
    };
}
