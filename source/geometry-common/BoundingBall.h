#pragma once

#include "BoundingBox.h"

template<typename Scalar, uint dimension>
class BoundingBall {
    private:
        Tuple<Scalar, dimension> center;
        Scalar squaredRadius;

        BoundingBall (const Tuple<Scalar, dimension>& _center) : center (_center), squaredRadius (-1) {}

        BoundingBall& addPoints (const Tuple<Scalar, dimension>* points, uint pointCount) {
            squaredRadius = -1;
            for (uint i = 0; i < pointCount; ++i) {
                Scalar squaredDelta = (points[i] - center).normL2Sq ();
                if (squaredDelta > squaredRadius) {
                    squaredRadius = squaredDelta;
                }
            }
            return *this;
        }

    public:
        BoundingBall (const Tuple<Scalar, dimension>& _center, Scalar radius) : center (_center), squaredRadius (radius * radius) {}

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

        // https://www.inf.ethz.ch/personal/emo/PublFiles/SmallEnclDisk_LNCS555_91.pdf
        // https://people.inf.ethz.ch/gaertner/subdir/texts/own_work/esa99_final.pdf
        // https://people.inf.ethz.ch/gaertner/subdir/software/miniball.html
        // https://people.inf.ethz.ch/gaertner/subdir/software/miniball/Miniball.hpp

        /*
        Algorithm 1.
        mtf mb(Ln, B):
            (* returns mb(Ln, B) *)
            mb := mb(B)
            IF |B| = d + 1 THEN
                RETURN mb
            END
            FOR i = 1 TO n DO
                IF p[i] not in mb THEN
                    mb := mtf mb(L[i - 1], B ∪ {p[i]})
                    update L by moving p[i] to the front
                END
            END
            RETURN mb



        */

        typedef list<Tuple<Scalar, dimension>> PointList;
        typedef typename PointList::iterator PointListIterator;

        static BoundingBall makeBall (const PointList& boundaryPoints) {
            int i, j;
            Tuple<Scalar, dimension> q0;
            Tuple<Scalar, dimension> c[dimension + 1];
            Tuple<Scalar, dimension> v[dimension + 1];
            Tuple<Scalar, dimension> a[dimension + 1];
            Scalar z[dimension + 1];
            Scalar f[dimension + 1];
            Scalar squaredRadius[dimension + 1];
            int fsize = boundaryPoints.size ();
            for (int fsize = 0; fsize < boundaryPoints.size(); ++fsize) {
                if (fsize == 0) {
                    c[0] = q0 = boundaryPoints[0];
                    squaredRadius[0] = 0;
                } else {
                    // set v_fsize to Q_fsize
                    v[fsize] = boundaryPoints[i] - q0;

                    // compute the a_{fsize,i}, i < fsize
                    for (i = 1; i < fsize; ++i) {
                        a[fsize][i] = 0;
                        for (j = 0; j < dimension; ++j) {
                            a[fsize][i] += v[i][j] * v[fsize][j];
                        }
                        a[fsize][i] *= (2 / z[i]);
                    }

                    // update v_fsize to Q_fsize-\bar{Q}_fsize
                    for (i = 1; i < fsize; ++i) {
                        for (j = 0; j < dimension; ++j) {
                            v[fsize][j] -= a[fsize][i] * v[i][j];
                        }
                    }

                    // compute z_fsize
                    z[fsize] =  2 * (v[fsize].normL2Sq ());

                    // reject push if z_fsize too small
                    /*
                    if (z[fsize] < (eps * currentSquaredRadius)) {
                        return false;
                    }
                    */

                    // update c, squaredRadius
                    //p=cit;
                    Scalar e = -squaredRadius[fsize-1] + (boundaryPoints[fsize] - c[fsize - 1]).normL2Sq ();
                    f[fsize] = e / z[fsize];

                    c[fsize] = c[fsize-1] + (f[fsize] * v[fsize]);
                    squaredRadius[fsize] = squaredRadius[fsize-1] + (e * f[fsize] / 2);
                }
                return BoundingBall (c[fsize], sqrt (squaredRadius[fsize]));
            }
            return BoundingBall (boundaryPoints[0], 0);
        }

        static BoundingBall algorithmMoveToFront (PointList& points, PointListIterator stop, PointList& boundaryPoints) {
            BoundingBall ball = makeBall (boundaryPoints);
            // see the paper for a proof that this is the limit of recursion
            if (boundaryPoints.size() != (dimension + 1)) {
                for (PointListIterator iter = points.begin (); iter != stop;) {
                    // let the iterator go on to the next element, but keep the reference to our
                    // current position. we do this because we may modify the list, and this will
                    // allow the iterator to finish traversing the rest of the list without impact.
                    PointListIterator current = iter++;

                    // check to see if the ball we have contains the current point
                    if (not ball.contains(*current)) {
                        // the current point is outside the ball, so we want to build a new ball
                        // that will contain it. add the current point to the boundary set, recur,
                        // and then remove the current point from the boundary set.
                        boundaryPoints.push_back(*current);
                        ball = algorithmMoveToFront (begin, current, boundaryPoints);
                        boundaryPoints.pop_back();

                        // move "current" to the front, so subsequent iterations over the points
                        // will encounter it early. on an intuitive level, this process sorts the
                        // point list by decreasing distance to the center of the optimal ball.
                        points.splice (points.begin(), points, current);
                    }
                }
            }
            return ball;
        }

        static BoundingBall makeFromAlgorithm1 (const Tuple<Scalar, dimension>* _points, uint pointCount) {
            // XXX TODO - randomly permute the points list
            PointList points (_points, _points + pointCount);
            PointList boundaryPoints;
            return algorithmMoveToFront (points, points.end(), &boundaryPoints);
        }

        // also see:
        // https://www.geometrictools.com/GTEngine/Include/Mathematics/GteMinimumAreaCircle2.h
        // http://www.cs.uu.nl/docs/vakken/ga/slides4b.pdf
        // http://www.stsci.edu/~RAB/Backup%20Oct%2022%202011/f_3_CalculationForWFIRSTML/Bob1.pdf
        /*
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
        */

        bool contains (const Tuple<Scalar, dimension>&point) const {
            return (point - center).normL2Sq () <= squaredRadius;
        }

        bool isEmpty () const {
            return squaredRadius < 0;
        }

        const Tuple<Scalar, dimension>& getCenter () const {
            return center;
        }

        Scalar getRadius () const {
            return  (squaredRadius >= 0) ? sqrt (squaredRadius) : -1;
        }
};
