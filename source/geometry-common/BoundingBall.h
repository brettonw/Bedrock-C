#pragma once

#include "BoundingBox.h"

template<typename Scalar, unsigned int dimension>
class BoundingBall {
    private:
        Tuple<Scalar, dimension> center;
        Scalar radius;
        unsigned int count;

        void addPoints (const Tuple<Scalar, dimension>* points, uint pointCount) {
            Scalar radiusSq = 0;
            for (uint i = 0; i < pointCount; ++i) {
                Scalar deltaSq = (points[i] - center).normL2Sq ();
                if (deltaSq > radiusSq) {
                    radiusSq = deltaSq;
                }
            }
            radius = sqrt (radiusSq);
            count = pointCount;
        }

    public:
        BoundingBall (const Tuple<Scalar, dimension>& _center, const Tuple<Scalar, dimension>* points, uint pointCount) : center (_center), radius (-1), count (0) {
            addPoints (points, pointCount);
        }

        BoundingBall (const Tuple<Scalar, dimension>* points, uint pointCount) : radius (-1), count (0) {
            // compute the center as the middle of a box
            BoundingBox<Scalar, dimension> box (points, pointCount);
            center = (box.getLow() + box.getHigh()) / 2;

            // now add the points...
            addPoints (points, pointCount);
        }

        unsigned int getCount () const {
            return count;
        }

        bool contains (const Tuple<Scalar, dimension>&point) {
            return (point - center).normL2Sq () <= (radius * radius);
        }

        const Tuple<Scalar, dimension>& getCenter () const {
            return center;
        }

        Scalar getRadius () const {
            return radius;
        }
};
