#pragma once

#include "Line3.h"

template<typename Scalar>
class Plane3 {
    private:
        Tuple3<Scalar> normal;
        Scalar D;

    public:
        Plane3 (const Tuple3<Scalar>& _normal, Scalar _D) : normal (_normal), D (_D) {}

        static Plane3<Scalar> PlaneFromPointNormal (const Tuple3<Scalar>& point, const Tuple3<Scalar>& normal) {
            return Plane3<Scalar> (normal, -(point DOT normal));
        }

        static Plane3<Scalar> PlaneFromThreePoints (const Tuple3<Scalar>& a, const Tuple3<Scalar>& b, const Tuple3<Scalar>& c) {
            Tuple3<Scalar> normal = ((b - a) CROSS (c - a)).normalized ();
            return PlaneFromPointNormal (a, normal);
        }

        static Plane3<Scalar> FitPlaneToPoints (const vector<Tuple3<Scalar> >& points) {
            // compute the mean point
            Tuple3<Scalar> mean;
            for (typename vector<Tuple3<Scalar> >::const_iterator iter = points.begin (); iter != points.end; ++iter) {
                mean += *iter;
            }
            mean /= points.size();

            // XXX this will need to have an SVD implementation
            // see: https://math.stackexchange.com/questions/99299/best-fitting-plane-given-a-set-of-points
            return PlaneFromPointNormal (mean, Tuple3<Scalar> (0, 1, 0));
        }

        // return the shortest scalar distance from a point to the plane (along the normal vector)
        Scalar distance (const Tuple3<Scalar>& point) const {
            return (point DOT normal) + D;
        }

        // check if a point is on the plane
        bool isOnPlane (const Tuple3<Scalar>& point) const {
            return distance (point) <= Tuple3<Scalar>::getEpsilon ();
        }

        // return the normal vector
        Tuple3<Scalar> getNormal () const {
            return normal;
        }

        // return the D component of the plane equation
        Scalar getD () const {
            return D;
        }

        // return a point that is on the plane
        Tuple3<Scalar> getPoint () const {
            return normal * -D;
        }

        // return a representation of the plane as a 4 vector
        Tuple4<Scalar> getPlane () const {
            return Tuple4<Scalar> (normal, D);
        }

        Scalar getIntersection (const Line3<Scalar>& line) {
            // compute the cosine of the angle between the line and the plane, then check to see if
            // that angle is ever going to reach the plane (i.e. it's not effectively parallel to
            // the plane). a parallel line get's an infinite time. otherwise, compute the time along
            // the line where the intersection will occur.
            Scalar cosTheta = -(line.getDirection () DOT normal);
            return (abs (cosTheta) < Tuple3<Scalar>::getEpsilon ()) ? numeric_limits<Scalar>::infinity () : distance (line.getOrigin) / cosTheta;
        }
};
