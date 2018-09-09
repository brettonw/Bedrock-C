#pragma once

#include "Line.h"

namespace Geometry3d {
    template<typename Scalar>
    class Plane {
        private:
            Tuple<Scalar, 3> normal;
            Scalar D;

        public:
            Plane (const Tuple<Scalar, 3>& _normal, Scalar _D) : normal (_normal), D (_D) {}

            static Plane<Scalar> fromPointNormal (const Tuple<Scalar, 3>& point, const Tuple<Scalar, 3>& normal) {
                return Plane<Scalar> (normal, -(point DOT normal));
            }

            static Plane<Scalar> fromThreePoints (const Tuple<Scalar, 3>& a, const Tuple<Scalar, 3>& b, const Tuple<Scalar, 3>& c) {
                Tuple<Scalar, 3> normal = ((b - a) CROSS (c - a)).normalized ();
                return fromPointNormal (a, normal);
            }

            static Plane<Scalar> fitPoints (const vector<Tuple<Scalar, 3> >& points) {
                // compute the mean point
                Tuple<Scalar, 3> mean;
                for (typename vector<Tuple<Scalar, 3> >::const_iterator iter = points.begin (); iter != points.end; ++iter) {
                    mean += *iter;
                }
                mean /= points.size();

                // XXX this will need to have an SVD implementation
                // see: https://math.stackexchange.com/questions/99299/best-fitting-plane-given-a-set-of-points
                return fromPointNormal (mean, Tuple<Scalar, 3> (0, 1, 0));
            }

            // return the shortest scalar distance from a point to the plane (along the normal vector)
            Scalar distance (const Tuple<Scalar, 3>& point) const {
                return (point DOT normal) + D;
            }

            // check if a point is on the plane
            bool isOnPlane (const Tuple<Scalar, 3>& point) const {
                return distance (point) <= Tuple<Scalar, 3>::getEpsilon ();
            }

            // return the normal vector
            const Tuple<Scalar, 3>& getNormal () const {
                return normal;
            }

            // return the D component of the plane equation
            Scalar getD () const {
                return D;
            }

            // return a point that is on the plane
            Tuple<Scalar, 3> getPoint () const {
                return normal * -D;
            }

            // return a representation of the plane as a 4 vector
            Tuple<Scalar, 4> getPlane () const {
                return Tuple<Scalar, 4> (normal, D);
            }

            Scalar getIntersection (const Line<Scalar, 3>& line) {
                // compute the cosine of the angle between the line and the plane, then check to see if
                // that angle is ever going to reach the plane (i.e. it's not effectively parallel to
                // the plane). a parallel line get's an infinite time. otherwise, compute the time along
                // the line where the intersection will occur.
                Scalar cosTheta = -(line.getDirection () DOT normal);
                return (abs (cosTheta) < Tuple<Scalar, 3>::getEpsilon ()) ? numeric_limits<Scalar>::infinity () : distance (line.getOrigin) / cosTheta;
            }
    };
}
