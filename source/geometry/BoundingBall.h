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

        static BoundingBall makeBall (const PointList& boundaryPoints) {
            // get the number of points
            uint n = boundaryPoints.size ();

            // if there are no points, or too many points to distinctly define a ball, so we return
            // an empty ball
            if ((n == 0) or (n > (dimension + 1))) {
                return BoundingBall ();
            }

            // there are some points, we'll want to iterate over them, at least the first one
            constPointListIterator iter = boundaryPoints.begin ();
            const Point& a = *iter++;

            // if it's only one point, return a ball with that one
            if (n == 1) {
                return BoundingBall (a, 0);
            }

            // in any n-dimension space, 2 points always define a ball with the center at the
            // middle of the line segment between them.
            const Point& b = *iter++;
            if (n == 2) {
                return fromTwoPoints (a, b);
            }

            // in any n-dimension space, any m = 2..n+1 points may be boundary points on a ball.
            // (more than n+1 is obviously possible...)
            // typical solvers create a set of linear equations using the unknown center of the
            // ball as a component that has to be solved for.
            // our general approach is based on the observation that n points define a hyperplane,
            // and the center of the ball must reside on a line perpendicular to that hyperplane,
            // with a known point at the median point of the boundary points that define the
            // hyperplane. given two set of n points, we can solve for the intersection of two
            // lines, or use trigonometry...
            const Point& c = *iter++;
            switch (dimension) {
                case 2: {
                    // we got here, n must be 3 - two midpoints
                    Vector ab = (b - a).normalized ();
                    Vector bc = (c - b).normalized ();
                    Point abMid = (a + b) * 0.5;
                    Point bcMid = (b + c) * 0.5;

                    // define a hyperplane from the first midpoint and a perpendicular vector
                    //Scalar C =
                    break;
                }

                case 3:

                    break;
            }
            return BoundingBall ();
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

        static BoundingBall makeFromAlgorithm1 (const Point* _points, uint pointCount) {
            // XXX TODO - randomly permute the points list
            PointList points (_points, _points + pointCount);
            PointList boundaryPoints;
            return algorithmMoveToFront (points, points.end(), &boundaryPoints);
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
