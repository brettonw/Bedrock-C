#pragma once

#include "Tuple.h"

namespace Geometry2d {
    template<typename Scalar>
    class Line {
        private:
            Tuple<Scalar, 2> normal;
            Scalar C;
    
        public:
            Line (const Tuple<Scalar, 2>& _normal, Scalar _C) : normal (_normal), C (_C) {}
    
            static Line<Scalar> fromPointNormal (const Tuple<Scalar, 2>& point, const Tuple<Scalar, 2>& normal) {
                Scalar C = -(point DOT normal);
                return Line<Scalar> (normal, C);
            }
    
            static Line<Scalar> fromTwoPoints (const Tuple<Scalar, 2>& a, const Tuple<Scalar, 2>& b) {
                Tuple<Scalar, 2> normal = perpendicular ((b - a).normalized ());
                return fromPointNormal (a, normal);
            }
    
            static Line<Scalar> fromSlopeIntercept (Scalar slope, Scalar yIntercept) {
                // compute two points on this line (y = mx + b)
                Tuple<Scalar, 2> a (-10, (slope * -10) + yIntercept);
                Tuple<Scalar, 2> b (10, (slope * 10) + yIntercept);
                return fromTwoPoints (a, b);
            }
    
            static Line<Scalar> fitPoints (const vector<Tuple<Scalar, 2> >& points) {
                Scalar intercept = 0;
                // compute the mean point
                Tuple<Scalar, 2> mean;
                for (typename vector<Tuple<Scalar, 2> >::const_iterator iter = points.begin (); iter != points.end; ++iter) {
                    mean += *iter;
                }
                mean /= points.size();
    
                // make a second pass to compute the sums needed to compute the slope
                Scalar numerator = 0;
                Scalar denominator = 0;
                for (typename vector<Tuple<Scalar, 2> >::const_iterator iter = points.begin (); iter != points.end; ++iter) {
                    Tuple<Scalar, 2> delta = *iter - mean;
                    numerator += delta[X] * delta[Y];
                    denominator += delta[X] * delta[X];
                }
    
                // compute the slope, and then the intercept
                if (denominator != 0) {
                    Scalar slope = numerator / denominator;
                    Scalar intercept = mean[Y] - (slope * mean[X]);
                    return LineFromSlopeIntercept (slope, intercept);
                }
                return Line<Scalar> (Tuple<Scalar, 2> (1, 0), -mean[X]);
            }
    
            // return the scalar perpendicular distance from a point to the line (along the normal vector)
            Scalar distance (const Tuple<Scalar, 2>& point) const {
                return (point DOT normal) + C;
            }
    
            // check if a point is on the line
            bool isOnLine (const Tuple<Scalar, 2>& point) const {
                return distance (point) <= Tuple<Scalar, 2>::getEpsilon ();
            }
    
            // return the normal vector
            const Tuple<Scalar, 2>& getNormal () const {
                return normal;
            }
    
            // return the C component of the line equation
            Scalar getC () const {
                return C;
            }
    
            // return a point that is on the line
            Tuple<Scalar, 2> getPoint () const {
                return normal * -C;
            }
    
            // return a representation of the line as a 3 vector
            Tuple<Scalar, 3> getLine () const {
                return Tuple<Scalar, 3> (normal, C);
            }

            // return the intersection of two lines
            Tuple<Scalar, 2> intersect (const Line<Scalar>& line) {

            }
    };
}
