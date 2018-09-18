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

    public:
        BoundingBall () : squaredRadius (-1) {}

        BoundingBall (const Point& _center) : center (_center), squaredRadius (0) {}

        BoundingBall (const Point& _center, Scalar _squaredRadius) : center (_center), squaredRadius (_squaredRadius) {}

        static BoundingBall fromTwoPoints (const Point& a, const Point& b) {
            // in any n-dimension space, 2 points always define a ball with the center at the
            // middle of the line segment between them.
            Point center = (a + b) / 2;
            return BoundingBall (center, (a - center).lengthSq ());
        }

        static BoundingBall fromPoints (const Point& center, const Point* points, uint pointCount) {
            return BoundingBall (center).addPoints(points, pointCount);
        }

        static BoundingBall fromPoints (const Point* points, uint pointCount) {
            // compute the center as the middle of a bounding box (median)
            BoundingBox<Scalar, dimension> box (points, pointCount);
            Point center = (box.getLow() + box.getHigh()) / 2;
            return BoundingBall (center).addPoints(points, pointCount);
        }

        static inline Point popBack (vector<Point>& vec) {
            Point point = vec.back ();
            vec.pop_back();
            return point;
        }

        static inline vector<Point>& pushBack (vector<Point>& vec, Point& point) {
            vec.push_back(point);
            return vec;
        }

        // primary references:
        // https://www.inf.ethz.ch/personal/emo/PublFiles/SmallEnclDisk_LNCS555_91.pdf
        // https://people.inf.ethz.ch/gaertner/subdir/texts/own_work/esa99_final.pdf
        // https://people.inf.ethz.ch/gaertner/subdir/software/miniball.html
        // https://people.inf.ethz.ch/gaertner/subdir/software/miniball/Miniball.hpp

        // also see:
        // https://www.geometrictools.com/GTEngine/Include/Mathematics/GteMinimumAreaCircle2.h
        // http://www.cs.uu.nl/docs/vakken/ga/slides4b.pdf
        // http://www.stsci.edu/~RAB/Backup%20Oct%2022%202011/f_3_CalculationForWFIRSTML/Bob1.pdf

        typedef list<Point> PointList;
        typedef typename PointList::iterator PointListIterator;
        typedef typename PointList::const_iterator constPointListIterator;

        ENABLE_DIMENSION(2)
        static BoundingBall makeBall (const PointList& boundaryPoints) {
            constPointListIterator iter = boundaryPoints.begin ();
            switch (boundaryPoints.size ()) {
                case 0:
                default: {
                    // no points, or the ball is over-specified - return an empty ball
                    return BoundingBall ();
                }
                case 1: {
                    // only one point is a zero-sized ball at that point
                    return BoundingBall (*iter);
                }
                case 2: {
                    // 2 points always define a ball with the center at the middle of the line
                    // segment between them
                    const Point& a = *iter++;
                    const Point& b = *iter;
                    return fromTwoPoints (a, b);
                }
                case 3: {
                    // define a hyperplane using the midpoint and direction of a-b, and a line using
                    // the midpoint of b-c and it's perpendicular direction
                    const Point& a = *iter++;
                    const Point& b = *iter++;
                    const Point& c = *iter;
                    auto hyperplane = Hyperplane2::fromPointNormal ((a + b) / 2, b - a);
                    Line2 line ((b + c) / 2, (c - b).perpendicular ());

                    // compute the intersection of the line with the plane, and Bob's your uncle
                    f8 t = hyperplane.intersect (line);
                    if (not isinf (t)) {
                        Point center = line.pointAt (t);
                        return BoundingBall (center, (a - center).lengthSq ());
                    } else {
                        // XXX do some work to figure out which point is collinear and exlude it
                    }
                }
            }
        }

        ENABLE_DIMENSION(3)
        static BoundingBall makeBall (const PointList& boundaryPoints) {
            constPointListIterator iter = boundaryPoints.begin ();
            switch (boundaryPoints.size ()) {
                case 0:
                default: {
                    // no points, or the ball is over-specified - return an empty ball
                    return BoundingBall ();
                }
                case 1: {
                    // only one point is a zero-sized ball at that point
                    return BoundingBall (*iter);
                }
                case 2: {
                    // 2 points always define a ball with the center at the middle of the line
                    // segment between them
                    const Point& a = *iter++;
                    const Point& b = *iter;
                    return fromTwoPoints (a, b);
                }
                case 3: {
                    return BoundingBall ();
                }
                case 4: {
                    return BoundingBall ();
                }
            }
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
                        ball = algorithmMoveToFront (points, current, boundaryPoints);
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

        static BoundingBall makeFromAlgorithm1 (const Point* _points, uint pointCount) {
            // XXX TODO - randomly permute the points list
            PointList points (_points, _points + pointCount);
            PointList boundaryPoints;
            return algorithmMoveToFront (points, points.end(), boundaryPoints);
        }


        bool contains (const Point&point) const {
            return (point - center).lengthSq () <= squaredRadius;
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

typedef BoundingBall<f8, 2> BoundingBall2;
typedef BoundingBall<f8, 3> BoundingBall3;
