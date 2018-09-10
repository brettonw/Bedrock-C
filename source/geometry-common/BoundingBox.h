#pragma once

#include "Tuple.h"

template<typename Scalar, unsigned int dimension>
class BoundingBox {
    private:
        Tuple<Scalar, dimension> low;
        Tuple<Scalar, dimension> high;
        unsigned int count;

    public:
        BoundingBox () : low (numeric_limits<Scalar>::max ()), high (numeric_limits<Scalar>::lowest ()), count (0) {}

        BoundingBox& addPoint (const Tuple<Scalar, dimension>& point) {
            TupleHelper<Scalar, dimension>::updateLowHigh (*point, *low, *high);
            ++count;
            return *this;
        }

        BoundingBox& addPoints (const Tuple<Scalar, dimension>* points, int count) {
            for (int i = 0; i < count; ++i) {
                addPoint (points[i]);
            }
            return *this;
        }

        Tuple<Scalar, dimension> getSpan () const {
            return high- low;
        }

        unsigned int getCount () const {
            return count;
        }

        bool contains (const Tuple<Scalar, dimension>&point) {
            return TupleHelper<Scalar, dimension>::inBound(*point, *low, *high);
        }

        const Tuple<Scalar, dimension>& getLow () const {
            return low;
        }

        const Tuple<Scalar, dimension>& getHigh () const {
            return high;
        }
};
