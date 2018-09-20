#pragma once

#include "BoundingBall.h"

#define TRC trace (depth * 2)
#define TRC1 trace ((depth * 2) + 1)

template<typename Scalar, uint dimension>
class Welzl {
    public:
        typedef Tuple<Scalar, dimension> Point;
        typedef Tuple<Scalar, dimension> Vector;
        typedef BoundingBall<Scalar, dimension> Ball;

    private:
        typedef list<Point> PointList;
        typedef typename PointList::iterator PointListIterator;

        PointList pointList;

        struct State {
            Point  p;   // point that was added to create this state
            Point  c;   // center point of ball
            Scalar r;   // squared radius of ball
            Vector v;
            Scalar z;
        };
        State states[dimension + 1];
        uint stateCount;

        static Log& trace (uint depth) {
            Log& log = Log::trace ();
            for (uint i = 0; i < depth; ++i) {
                log << "  ";
            }
            return log;
        }

        Welzl (const Point* points, uint pointCount) : stateCount (0) {
            vector<Point> shuffledPoints (points, points + pointCount);
            random_device randomNumberGenerator;
            mt19937 twister(randomNumberGenerator ());
            shuffle(shuffledPoints.begin(), shuffledPoints.end(), twister);

            // put the points into our list to make splicing to front fast, and then start
            copy(shuffledPoints.begin(), shuffledPoints.end(), back_inserter(pointList));
        };

        // this is an implementation of the incremental solution to the k-dimensional minimum ball
        // with points on the boundary described in the Welzl and Gaertner papers. The best
        // discussion is Section 4 "The implementation" in Gaertner's "Fast and Robust Smallest
        // Enclosing Balls", which is available at:
        //   https://people.inf.ethz.ch/gaertner/subdir/texts/own_work/esa99_final.pdf
        bool  addBoundaryPoint (const Point& point) {
            State& current = states[stateCount];

            // this is really just for bookkeeping
            current.p = point;

            // if this is the first boundary point to be added
            if (stateCount == 0) {
                // use the point we just added as the center, with squared radius 0
                current.c = point;
                current.r = 0;
            } else {
                State& previous = states[stateCount - 1];

                // v_current is the vector delta between the current point and the first center
                current.v = point - states[0].c;

                // setup for the second half of equation 11
                Scalar a[dimension + 1];
                for (uint i = 1; i < stateCount; ++i) {
                    a[i] = (states[i].v DOT current.v) * (2 / states[i].z);
                }

                // update v_current to Q_current - Qbar_current, see discussion of equation 9
                for (uint i = 1; i < stateCount; ++i) {
                    current.v -= a[i] * states[i].v;
                }

                // compute z_current, lemma 1 (iii), twice the distance from Qm to its
                // projection, and check if it is not too small
                current.z = current.v.lengthSq () * 2;
                if (current.z < (Point::getEpsilon () * previous.r)) {
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
                    Scalar e = (point - previous.c).lengthSq () - previous.r;
                    current.c = previous.c + ((e * current.v) / current.z);
                    current.r = previous.r + ((e * e) / (2 * current.z));
                }
            }

            // the add is valid, keep it
            ++stateCount;
            return true;
        }

        void popBoundaryPoint () {
            --stateCount;
        }

        Ball getBoundingBall () const {
            return ((stateCount > 0) && (states[stateCount - 1].valid)) ? Ball::fromCenterSquaredRadius(states[stateCount - 1].c, states[stateCount - 1].r) : Ball ();
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
        Ball algorithmMoveToFront (PointListIterator stop) {
            uint depth = stateCount;
            TRC << "algorithmMoveToFront (with " << stateCount << " point" << ((stateCount == 1) ? "" : "s") << " on boundary) - " << endl;

            // get the current bounding ball
            Ball ball = getBoundingBall ();
            TRC1 << "Ball = " << ball << endl;

            // see the paper for a proof that this is the limit of recursion
            if (stateCount != (dimension + 1)) {
                TRC << "Testing points" << endl;
                for (PointListIterator iter = pointList.begin (); iter != stop;) {
                    // let the iterator go on to the next element, but keep the reference to our
                    // current position. we do this because we may modify the list, and this will
                    // allow the iterator to finish traversing the rest of the list without impact.
                    PointListIterator current = iter++;

                    // check to see if the ball we have contains the current point
                    Log& trc1 = TRC1 << "Point - " << *current;
                    if ((not ball.contains(*current)) and addBoundaryPoint (*iter)) {
                        trc1 << " - NOT CONTAINED" << endl;
                        // the current point is outside the ball, so we want to build a new ball
                        // that will contain it. add the current point to the boundary set, recur,
                        // and then remove the current point from the boundary set.
                        ball = algorithmMoveToFront (current);
                        popBoundaryPoint ();
                        TRC1 << "  Ball = " << ball << endl;

                        // move "current" to the front, so subsequent iterations over the points
                        // will encounter it early. on an intuitive level, this process sorts the
                        // point list by decreasing distance to the center of the optimal ball.
                        pointList.splice (pointList.begin(), pointList, current);
                    } else {
                        trc1 << " - contained" << endl;
                    }
                }
                TRC << "Finished testing points" << endl;
            } else {
                TRC << "Skipping points" << endl;
            }
            return ball;
        }

        // STATIC VERSION
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
        static Ball algorithmMoveToFront (PointList& points, PointListIterator stop, vector<Point>& boundaryPoints, Ball& ball) {
            uint depth = boundaryPoints.size ();
            TRC << "algorithmMoveToFront (with " << boundaryPoints.size () << " point" << ((boundaryPoints.size () == 1) ? "" : "s") << " on boundary) - " << endl;
            for (typename vector<Point>::iterator iter = boundaryPoints.begin (); iter != boundaryPoints.end (); ++ iter) {
                TRC1 << "Boundary Point: " << *iter << endl;
            }

            // try to create a new bounding ball from the boundary points, and check if it succeeds
            if (Ball::fromBoundaryPoints (boundaryPoints.data(), boundaryPoints.size(), ball)) {
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

        Ball fromPoints () {
            return algorithmMoveToFront (pointList.end ());
        }

    public:
        static Ball fromPointsB (const Point* points, uint pointCount) {
            return Welzl (points, pointCount).fromPoints();
        }

        static Ball fromPoints (const Point* points, uint pointCount) {
            // make a shuffled copy of the points
            vector<Point> shuffledPoints (points, points + pointCount);
            random_device randomNumberGenerator;
            mt19937 twister(randomNumberGenerator ());
            shuffle(shuffledPoints.begin(), shuffledPoints.end(), twister);

            // put the points into a list to make splicing to front fast, and then start
            PointList pointList (shuffledPoints.begin (), shuffledPoints.end ());
            vector<Point> boundaryPoints;
            Ball ball;
            return algorithmMoveToFront (pointList, pointList.end(), boundaryPoints, ball);
        }
};

#undef TRC
#undef TRC1

typedef Welzl<f8, 2> Welzl2;
typedef Welzl<f8, 3> Welzl3;
