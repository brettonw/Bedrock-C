#pragma once

#include "Tuple.h"

template<typename Scalar, uint dimension>
class BoundingBox {
    private:
        Tuple<Scalar, dimension> low;
        Tuple<Scalar, dimension> high;

    public:
        BoundingBox () : low (numeric_limits<Scalar>::max ()), high (numeric_limits<Scalar>::lowest ()) {}

        BoundingBox (const Tuple<Scalar, dimension>* points, uint pointCount) : low (numeric_limits<Scalar>::max ()), high (numeric_limits<Scalar>::lowest ()) {
            addPoints (points, pointCount);
        }

        BoundingBox& addPoint (const Tuple<Scalar, dimension>& point) {
            TupleHelper<Scalar, dimension>::updateLowHigh (*point, *low, *high);
            return *this;
        }

        BoundingBox& addPoints (const Tuple<Scalar, dimension>* points, uint pointCount) {
            for (uint i = 0; i < pointCount; ++i) {
                addPoint (points[i]);
            }
            return *this;
        }

        static BoundingBox fromPoints (const Tuple<Scalar, dimension>* points, uint pointCount) {
            return BoundingBox ().addPoints(points, pointCount);
        }

        Tuple<Scalar, dimension> getSpan () const {
            return high- low;
        }

        bool contains (const Tuple<Scalar, dimension>&point) const {
            return TupleHelper<Scalar, dimension>::inBound(*point, *low, *high);
        }

        bool isEmpty () const {
            Tuple<Scalar, dimension> span = getSpan ();
            return (span[X] < 0) or (span[Y] < 0);
        }

        const Tuple<Scalar, dimension>& getLow () const {
            return low;
        }

        const Tuple<Scalar, dimension>& getHigh () const {
            return high;
        }
};
