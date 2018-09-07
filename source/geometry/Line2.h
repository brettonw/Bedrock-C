#pragma once

#include "Tuple.h"

template<typename Scalar>
class Line2 {
    private:
        Tuple2<Scalar> normal;
        Scalar C;

    public:
        Line2 (const Tuple2<Scalar>& _normal, Scalar _C) : normal (_normal), C (_C) {}

        static Line2<Scalar> LineFromTwoPoints (const Tuple2<Scalar>& a, const Tuple2<Scalar>& b) {
            Tuple2<Scalar> normal = (b - a).normalized ().perpendicular ();
            Scalar C = -(a DOT normal);
            return Line2<Scalar> (normal, C);
        }

        static Line2<Scalar> LineFromPointNormal (const Tuple2<Scalar>& point, const Tuple2<Scalar>& normal) {
            Scalar C = -(point DOT normal);
            return Line2<Scalar> (normal, C);
        }

        static Line2<Scalar> LineFromSlopeIntercept (Scalar slope, Scalar yIntercept) {
            // compute two points on this line (y = mx + b)
            Tuple2<Scalar> a (-10, (slope * -10) + yIntercept);
            Tuple2<Scalar> b (10, (slope * 10) + yIntercept);
            return LineFrom2Points (a, b);
        }

        static Line2<Scalar> FitLineToPoints (const vector<Tuple2<Scalar> >& points) {
            Scalar intercept = 0;
            // compute the mean point
            Tuple2<Scalar> mean;
            for (typename vector<Tuple2<Scalar> >::const_iterator iter = points.begin (); iter != points.end; ++iter) {
                mean += *iter;
            }
            mean /= points.size();

            // make a second pass to compute the sums needed to compute the slope
            Scalar numerator = 0;
            Scalar denominator = 0;
            for (typename vector<Tuple2<Scalar> >::const_iterator iter = points.begin (); iter != points.end; ++iter) {
                Tuple2<Scalar> delta = *iter - mean;
                numerator += delta[X] * delta[Y];
                denominator += delta[X] * delta[X];
            }

            // compute the slope, and then the intercept
            if (denominator != 0) {
                Scalar slope = numerator / denominator;
                Scalar intercept = mean[Y] - (slope * mean[X]);
                return LineFromSlopeIntercept (slope, intercept);
            }
            return Line2<Scalar> (Tuple2<Scalar> (1, 0), -mean[X]);
        }

        // return the shortest scalar distance from a point to the line (along the normal vector)
        Scalar distance (const Tuple2<Scalar>& point) const {
            return (point DOT normal) + C;
        }

        // check if a point is on the line
        bool isOnLine (const Tuple2<Scalar>& point) const {
            return distance (point) <= Tuple2<Scalar>::getEpsilon ();
        }

        // return the normal vector
        Tuple2<Scalar> getNormal () const {
            return normal;
        }

        // return the C component of the line equation
        Scalar getC () const {
            return C;
        }

        // return a point that is on the line
        Tuple2<Scalar> getPoint () const {
            return normal * -C;
        }

        // return a representation of the line as a 3 vector
        Tuple3<Scalar> getLine () const {
            return Tuple3<Scalar> (normal, C);
        }
};
