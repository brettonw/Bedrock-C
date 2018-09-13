#pragma once

#include "BoundingBox.h"

template<typename Scalar, uint dimension>
class BoundingBall {
    private:
        Tuple<Scalar, dimension> center;
        Scalar radius;

        BoundingBall (const Tuple<Scalar, dimension>& _center) : center (_center), radius (-1) {}

        BoundingBall& addPoints (const Tuple<Scalar, dimension>* points, uint pointCount) {
            Scalar radiusSq = 0;
            for (uint i = 0; i < pointCount; ++i) {
                Scalar deltaSq = (points[i] - center).normL2Sq ();
                if (deltaSq > radiusSq) {
                    radiusSq = deltaSq;
                }
            }
            radius = sqrt (radiusSq);
            return *this;
        }

    public:
        BoundingBall (const Tuple<Scalar, dimension>& _center, Scalar _radius) : center (_center), radius (_radius) {}

        static BoundingBall fromPoints (const Tuple<Scalar, dimension>& center, const Tuple<Scalar, dimension>* points, uint pointCount) {
            return BoundingBall (center).addPoints(points, pointCount);
        }

        static BoundingBall fromPoints (const Tuple<Scalar, dimension>* points, uint pointCount) {
            // compute the center as the middle of a bounding box (median)
            BoundingBox<Scalar, dimension> box (points, pointCount);
            Tuple<Scalar, dimension> center = (box.getLow() + box.getHigh()) / 2;
            return BoundingBall (center).addPoints(points, pointCount);
        }

        static inline Tuple<Scalar, dimension> popBack (vector<Tuple<Scalar, dimension>>& v) {
            Tuple<Scalar, dimension> t = v.back ();
            v.pop_back();
            return v;
        }

        static inline vector<Tuple<Scalar, dimension>>& pushBack (vector<Tuple<Scalar, dimension>>& v, Tuple<Scalar, dimension>& t) {
            v.push_back(t);
            return v;
        }

        // https://people.inf.ethz.ch/gaertner/subdir/texts/own_work/esa99_final.pdf
        // https://people.inf.ethz.ch/gaertner/subdir/software/miniball.html
        // https://people.inf.ethz.ch/gaertner/subdir/software/miniball/Miniball.hpp

        // also see:
        // https://www.geometrictools.com/GTEngine/Include/Mathematics/GteMinimumAreaCircle2.h
        // http://www.cs.uu.nl/docs/vakken/ga/slides4b.pdf
        // http://www.stsci.edu/~RAB/Backup%20Oct%2022%202011/f_3_CalculationForWFIRSTML/Bob1.pdf
        static BoundingBall welzl (vector<Tuple<Scalar, dimension>>& p, vector<Tuple<Scalar, dimension>>& r) {
            // this is a recursive function on the number of points, but maybe that could be converted to a tail recursion
            // p is a candidate set, assumed pre-shuffled
            // r is the set of points that are on the boundary of the current circle

            // termination step
            if ((p.size() == 0) or (r.size () >= 3)) {
                uint rSize = r.size ();
                if (rSize == 1) {
                    return BoundingBall (r[0], 0);
                }
                if (rSize == 2) {
                    Tuple<Scalar, dimension> delta = r[0] - r[1];
                    return BoundingBall ((r[0] + r[1]) * 0.5, (r[0] - r[1]).length ());
                }

                // shortcut
                Tuple<Scalar, dimension> mean = (r[0] + r[1] + r[2]) / 3;
                return BoundingBall (mean, (r[0] - mean).length ());
            }

            // recursion step
            Tuple<Scalar, dimension> rho = popBack(p);
            BoundingBall bb = welzl (p, r);
            return (bb.contains(rho)) ? bb : welzl (p, pushBack (r, rho));
        }

        static BoundingBall fromPointsWelzl (const Tuple<Scalar, dimension>* points, uint pointCount) {
            // make the pointer into a vector
            vector<Tuple<Scalar, dimension>> p (points, points + pointCount);
            vector<Tuple<Scalar, dimension>> r;
            return welzl (p, r);

        }

        bool contains (const Tuple<Scalar, dimension>&point) const {
            return (point - center).normL2Sq () <= (radius * radius);
        }

        bool isEmpty () const {
            return radius < 0;
        }

        const Tuple<Scalar, dimension>& getCenter () const {
            return center;
        }

        Scalar getRadius () const {
            return radius;
        }
};
