#pragma once

#include "Tuple.h"

template<typename Scalar>
class Line3 {
    private:
        Tuple3<Scalar> origin;
        Tuple3<Scalar> direction;

    public:
        Line3 (const Tuple3<Scalar>& _origin, const Tuple3<Scalar>& _direction) : origin (_origin), direction (_direction) {}

        static Line3<Scalar> LineFromTwoPoints (const Tuple3<Scalar>& a, const Tuple3<Scalar>& b) {
            return Line3<Scalar> (a, (b - a).normalized ());
        }

        // return the shortest scalar distance from a point to the line (along a perpendicular vector)
        Scalar distance (const Tuple3<Scalar>& point) const {
            // the wolfram method - http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
            return sqrt ((direction CROSS (origin - point)).normL2Sq () / direction.normL2Sq ());
        }

        bool isOnLine (const Tuple3<Scalar>& point) const {
            return distance (point) <= Tuple3<Scalar>::getEpsilon ();
        }

        Tuple3<Scalar>& getOrigin () const {
            return origin;
        }

        Tuple3<Scalar>& getDirection () const {
            return direction;
        }

        Tuple3<Scalar> getPoint (Scalar t) {
            return origin + (direction * t);
        }
};
