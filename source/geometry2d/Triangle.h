#pragma once

#include "Plane3.h"

template<typename Scalar>
class Triangle2 {
    private:
        const Tuple2<Scalar>* points;

        static bool sameSide (const Tuple2<Scalar>& point, const Tuple2<Scalar>& a, const Tuple2<Scalar>& b) {
            // this is the raw line test operation
            return ((b - a) CROSS (point - a)) > 0;
        }

    public:
        Triangle2 (const Tuple2<Scalar>* _points) : points (_points) {}

        Scalar area () const {
            return ((points[2] - points[0]) CROSS (points[1] - points[0])) / 2;
        }

        bool contains (const Tuple2<Scalar>& point) const {


            unction SameSide(p1,p2, a,b)
                cp1 = CrossProduct(b-a, p1-a)
                cp2 = CrossProduct(b-a, p2-a)
                if DotProduct(cp1, cp2) >= 0 then return true
                else return false

            function PointInTriangle(p, a,b,c)
                if SameSide(p,a, b,c) and SameSide(p,b, a,c)
                    and SameSide(p,c, a,b) then return true
                else return false
        }
};
