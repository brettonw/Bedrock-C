#pragma once

#include "BoundingBox.h"
#include "Hyperplane.h"

template<typename Scalar, uint dimension>
class BoundingBall {
    public:
        typedef Tuple<Scalar, dimension> Point;
        typedef Tuple<Scalar, dimension> Vector;

    private:
        Point center;
        Scalar squaredRadius;

        class BuilderState {
            public:
                Point  b;   // boundary point
                Point  c;   // center point of updated ball
                Scalar r;   // squared radius of updated ball

                Vector v;
                Vector a;
                Scalar z;
                Scalar f;
        };

        class Builder {
            private:
                BuilderState states[dimension + 1];
                uint stateCount;

            public:
                Builder () : stateCount (0) {};

                bool  addBoundaryPoint (const Point& point) {
                    BuilderState& current = states[stateCount];

                    // this is really just for bookkeeping
                    current.b = point;

                    // if this is the first boundary point to be added
                    if (stateCount == 0) {
                        // use the point we just added as the center, with squared radius 0
                        current.c = point;
                        current.r = 0;

                        // the add is valid, keep it
                        ++stateCount;
                    } else {
                        BuilderState& previous = states[stateCount - 1];

                        // set v_fsize to Q_fsize
                        current.v = point - states[0].c;

                        // compute the a_{current,i}, i < current, equation 9
                        for (uint i = 1; i < stateCount; ++i) {
                            current.a[i] = (states[i].v DOT current.v) * (2 / states[i].z);
                        }

                        // update v_fsize to Q_fsize-\bar{Q}_fsize
                        for (uint i = 1; i < stateCount; ++i) {
                            for (uint j = 0; j < dimension; ++j) {
                                current.v[j] -= current.a[i] * states[i].v[j];
                            }
                        }

                        // compute z_fsize, lemma 1 (iii)
                        current.z = current.v.lengthSq () * 2;

                        // reject push if z_fsize too small
                        if (current.z >= (Point::getEpsilon () * previous.r)) {
                            // update c, squaredRadius
                            Scalar e = -previous.r;
                            for (uint i = 0; i < dimension; ++i) {
                                e += square<Scalar> (states[i].b - previous.c[i]);
                            }

                            // equation 10
                            current.f = e / current.z;
                            current.r = previous.r + (e * current.f / 2);

                            for (uint i = 0; i < dimension; ++i) {
                                current.c[i] = previous.c[i] + (current.f * current.v[i]);
                            }

                            // the add is valid, keep it
                            ++stateCount;
                        }
                    }
                    return *this;
                }

                BoundingBall getBoundingBall () const {
                    return ((stateCount > 0) && (states[stateCount - 1].valid)) ? BoundingBall (states[stateCount - 1].c, states[stateCount - 1].r) : BoundingBall ();
                }

                Point& getCurrentCenter () const {
                    return ((stateCount > 0) && (states[stateCount - 1].valid)) ? states[stateCount - 1].c : Point ();
                }

                Scalar getCurrentRadius () const {
                    return ((stateCount > 0) && (states[stateCount - 1].valid)) ? states[stateCount - 1].r : -1;
                }

                Builder& pop () {
                    --stateCount;
                    return *this;
                }
        };

        BoundingBall (const Point& _center, Scalar _squaredRadius) : center (_center), squaredRadius (_squaredRadius) {}

        BoundingBall& addPoints (const Point* points, uint pointCount) {
            squaredRadius = -1;
            for (uint i = 0; i < pointCount; ++i) {
                Scalar squaredDelta = (points[i] - center).lengthSq ();
                if (squaredDelta > squaredRadius) {
                    squaredRadius = squaredDelta;
                }
            }
            return *this;
        }

        // primary references:
        // https://people.inf.ethz.ch/gaertner/subdir/texts/own_work/esa99_final.pdf
        // https://people.inf.ethz.ch/gaertner/subdir/software/miniball.html
        // https://people.inf.ethz.ch/gaertner/subdir/software/miniball/Miniball.hpp

        // also see:
        // https://www.geometrictools.com/GTEngine/Include/Mathematics/GteMinimumAreaCircle2.h
        // http://www.cs.uu.nl/docs/vakken/ga/slides4b.pdf
        // https://onlinelibrary.wiley.com/doi/pdf/10.1002/nav.3800210414

        typedef list<Point> PointList;
        typedef typename PointList::iterator PointListIterator;

        static Log& trace (uint depth) {
            Log& log = Log::trace ();
            for (uint i = 0; i < depth; ++i) {
                log << "  ";
            }
            return log;
        }
        #define TRC trace (depth * 2)
        #define TRC1 trace ((depth * 2) + 1)

        static BoundingBall fromBoundaryPointsX (const Point* boundaryPoints, uint boundaryPointCount) {
            switch (boundaryPointCount) {
                case 1: return BoundingBall (boundaryPoints[0]);
                case 2: return fromTwoPoints (boundaryPoints[0], boundaryPoints[1]);
                case 3: return fromThreePoints (boundaryPoints[0], boundaryPoints[1], boundaryPoints[2]);
                case 4: { }
                default: break;
            }
            return BoundingBall ();
        }

        // abstracted from the miniball code, this is an implementation of the incremental solution
        // to the k-dimensional minimum ball with points on the boundary described in the Welzl and
        // Gaertner papers. The best discussion is Section 4 "The implementation" in Gaertner's
        // "Fast and Robust Smallest Enclosing Balls", which is available at:
        //   https://people.inf.ethz.ch/gaertner/subdir/texts/own_work/esa99_final.pdf
        static bool fromBoundaryPoints (const Point* boundaryPoints, uint boundaryPointCount, BoundingBall& ball) {
            Scalar epsilon = square<Scalar>(Point::getEpsilon());
            if (boundaryPointCount > 0) {
                Vector v[dimension + 1];
                Scalar z[dimension + 1];

                Scalar squaredRadius = 0;
                Point  center = boundaryPoints[0];

                for (uint current = 1; current < boundaryPointCount; ++current) {
                    // v_current is the vector delta between the current point and the first point
                    v[current] = boundaryPoints[current] - boundaryPoints[0];

                    // setup for the second half of equation 11
                    Vector a;
                    for (uint i = 1; i < current; ++i) {
                        a[i] = (v[i] DOT v[current]) * (2 / z[i]);
                    }

                    // update v_current to Q_current - Qbar_current, see discussion of equation 9
                    for (uint i = 1; i < current; ++i) {
                        for (uint j = 0; j < dimension; ++j) {
                            v[current][j] -= a[i] * v[i][j];
                        }
                    }

                    // compute z_current, lemma 1 (iii), twice the distance from Qm to its
                    // projection, and check if it is not too small
                    z[current] = v[current].lengthSq () * 2;
                    if (z[current] < (epsilon * squaredRadius)) {
                        // see discussion around equation 12. skip if z is too small, because in
                        // the next step, we divide by z. if z is a very small value, that will
                        // result in numeric instability. this could happen if two points on the
                        // boundary are very close to each other (or the same point repeated). we
                        // effectively treat the new point as if it is inside the ball we have,
                        // and accept the possible error.
                        Log::warn () << "z < epsilon" << endl;
                        return false;
                    } else {
                        // equation 9/10
                        Scalar e = (boundaryPoints[current] - center).lengthSq () - squaredRadius;
                        center += (e * v[current]) / z[current];
                        squaredRadius += (e * e) / (2 * z[current]);
                    }
                }
                ball = BoundingBall (center, squaredRadius);
            }
            return true;
        }

        // algorithm first described by Emo Welzl, in his paper "Smallest enclosing disks", which
        // is available (with figures) at:
        //   http://www.stsci.edu/~RAB/Backup%20Oct%2022%202011/f_3_CalculationForWFIRSTML/Bob1.pdf
        // and a slightly different version is available from the author directly, at:
        //   https://www.inf.ethz.ch/personal/emo/PublFiles/SmallEnclDisk_LNCS555_91.pdf
        // an extension to this algorithm is presented by Bernd Gaertner, with a better description
        // of the iterative ball construction, available at:
        //   https://people.inf.ethz.ch/gaertner/subdir/texts/own_work/esa99_final.pdf
        // and Gaertner provides a functional if not obfuscated implementation of "miniball" at:
        //   https://people.inf.ethz.ch/gaertner/subdir/software/miniball.html
        // a visual depiction of computing Ball[i] from Ball[i-1] + p is at:
        //   http://www.cs.uu.nl/docs/vakken/ga/slides4b.pdf
        static BoundingBall algorithmMoveToFront (PointList& points, PointListIterator stop, vector<Point>& boundaryPoints, BoundingBall& ball) {
            uint depth = boundaryPoints.size ();
            TRC << "algorithmMoveToFront (with " << boundaryPoints.size () << " point" << ((boundaryPoints.size () == 1) ? "" : "s") << " on boundary) - " << endl;
            for (typename vector<Point>::iterator iter = boundaryPoints.begin (); iter != boundaryPoints.end (); ++ iter) {
                TRC1 << "Boundary Point: " << *iter << endl;
            }

            // try to create a new bounding ball from the boundary points, and check if it succeeds
            if (fromBoundaryPoints (boundaryPoints.data(), boundaryPoints.size(), ball)) {
                TRC1 << "Ball = " << ball << endl;

                // see the paper for a proof that this is the limit of recursion
                if (boundaryPoints.size() != (dimension + 1)) {
                    TRC << "Testing points" << endl;
                    for (PointListIterator iter = points.begin (); iter != stop;) {
                        // let the iterator go on to the next element, but keep the reference to our
                        // current position. we do this because we may modify the list, and this will
                        // allow the iterator to finish traversing the rest of the list without impact.
                        PointListIterator current = iter++;

                        // check to see if the ball we have contains the current point
                        Log& trc1 = TRC1 << "Point - " << *current;
                        if (not ball.contains(*current)) {
                            trc1 << " - NOT CONTAINED" << endl;
                            // the current point is outside the ball, so we want to build a new ball
                            // that will contain it. add the current point to the boundary set, recur,
                            // and then remove the current point from the boundary set.
                            boundaryPoints.push_back(*current);
                            ball = algorithmMoveToFront (points, current, boundaryPoints, ball);
                            TRC1 << "  Ball = " << ball << endl;
                            boundaryPoints.pop_back();

                            // move "current" to the front, so subsequent iterations over the points
                            // will encounter it early. on an intuitive level, this process sorts the
                            // point list by decreasing distance to the center of the optimal ball.
                            points.splice (points.begin(), points, current);
                        } else {
                            trc1 << " - contained" << endl;
                        }
                    }
                    TRC << "Finished testing points" << endl;
                } else {
                    TRC << "Skipping points" << endl;
                }
            }
            return ball;
        }

    public:
        BoundingBall () : squaredRadius (-1) {}

        BoundingBall (const Point& _center) : center (_center), squaredRadius (0) {}

        static BoundingBall fromCenterRadius (const Point& center, Scalar radius) {
            return BoundingBall (center, radius * radius);
        }

        static BoundingBall fromCenterSquaredRadius (const Point& center, Scalar squaredRadius) {
            return BoundingBall (center, squaredRadius);
        }

        // in any n-dimension space, 2 points always define a ball with the center at the middle of
        // the line segment between them.
        static BoundingBall fromTwoPoints (const Point& a, const Point& b) {
            Point center = (a + b) / 2;
            return BoundingBall (center, (a - center).lengthSq ());
        }

        ENABLE_DIMENSION(2)
        static BoundingBall fromThreePoints (const Point& a, const Point& b, const Point& c) {
            // define a hyperplane using the midpoint and direction of a-b, and a line using
            // the midpoint of b-c and it's perpendicular direction
            auto hyperplane = Hyperplane2::fromPointNormal ((a + b) / 2, b - a);
            Line2 line ((b + c) / 2, (c - b).perpendicular ());

            // compute the intersection of the line with the plane, and Bob's your uncle
            f8 t = hyperplane.intersect (line);
            if (not std::isinf (t)) {
                Point center = line.pointAt (t);
                return BoundingBall (center, (a - center).lengthSq ());
            } else {
                // the points are collinear, or two of the points are equivalent
                // XXX do some work to figure out which point is collinear and exlude it
            }
            return BoundingBall ();
        }

        // sometimes, you want to fix the center and then just add all the points - for instance
        // when bounding a physical object whose center of gravity is know, but which would not be
        // the same center as a geometric center
        static BoundingBall fromCenterAndPoints (const Point& center, const Point* points, uint pointCount) {
            return BoundingBall (center).addPoints(points, pointCount);
        }

        static BoundingBall fromPoints (const Point* points, uint pointCount) {
            // make a shuffled copy of the points
            vector<Point> shuffledPoints (points, points + pointCount);
            random_device randomNumberGenerator;
            mt19937 twister(randomNumberGenerator ());
            shuffle(shuffledPoints.begin(), shuffledPoints.end(), twister);

            // put the points into a list to make splicing to front fast, and then start
            PointList pointList (shuffledPoints.begin (), shuffledPoints.end ());
            vector<Point> boundaryPoints;
            BoundingBall ball;
            return algorithmMoveToFront (pointList, pointList.end(), boundaryPoints, ball);
        }


        bool contains (const Point&point) const {
            return (point - center).lengthSq () <= (squaredRadius + Point::getEpsilon());
        }

        bool isEmpty () const {
            return squaredRadius < 0;
        }

        const Point& getCenter () const {
            return center;
        }

        Scalar getSquaredRadius () const {
            return  squaredRadius;
        }

        Scalar getRadius () const {
            return  (squaredRadius >= 0) ? sqrt (squaredRadius) : -1;
        }
};

// ostream writer...
template<typename Scalar, uint dimension>
ostream& operator << (ostream& stream, const BoundingBall<Scalar, dimension>& ball) {
    return stream << "center: " << ball.getCenter() << ", radius: " << ball.getRadius();
}

typedef BoundingBall<f8, 2> BoundingBall2;
typedef BoundingBall<f8, 3> BoundingBall3;
