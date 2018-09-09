#pragma once

#include "Tuple.h"

template<typename Scalar, unsigned int dimension>
class Line {
    private:
        Tuple<Scalar, dimension> origin;
        Tuple<Scalar, dimension> direction;

    public:
        Line (const Tuple<Scalar, dimension>& _origin, const Tuple<Scalar, dimension>& _direction) : origin (_origin), direction (_direction) {}

        static Line fromTwoPoints (const Tuple<Scalar, dimension>& a, const Tuple<Scalar, dimension>& b) {
            return Line<Scalar, dimension> (a, (b - a).normalized ());
        }

        // return the shortest scalar distance from a point to the line (along a perpendicular vector)
        template <int dim = dimension, typename std::enable_if<dim == 2, void>::type* = nullptr>
        Scalar distance (const Tuple<Scalar, dimension>& point) const {
            return direction CROSS (origin - point);
        }

        // return the shortest scalar distance from a point to the line (along a perpendicular vector)
        template <int dim = dimension, typename std::enable_if<dim == 3, void>::type* = nullptr>
        Scalar distance (const Tuple<Scalar, dimension>& point) const {
            return (direction CROSS (origin - point)).norm ();
        }

        bool isOnLine (const Tuple<Scalar, dimension>& point) const {
            return distance (point) <= Tuple<Scalar, dimension>::getEpsilon ();
        }

        Tuple<Scalar, dimension>& getOrigin () const {
            return origin;
        }

        Tuple<Scalar, dimension>& getDirection () const {
            return direction;
        }

        Tuple<Scalar, dimension> getPoint (Scalar t) {
            return origin + (direction * t);
        }
};
