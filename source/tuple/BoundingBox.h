#pragma once

#include "Tuple.h"

template<typename Scalar, unsigned int dimension>
class BoundingBox {
    private:
        Tuple<Scalar, dimension> low;
        Tuple<Scalar, dimension> high;

    public:
        BoundingBox () : low (numeric_limits<Scalar>::max ()), high (numeric_limits<Scalar>::lowest ()) {}

        BoundingBox& addPoint (const Tuple<Scalar, dimension>& point) {
            for (int i = 0; i < dimension; ++i) {
                low[i] = min (low[i], point[i]);
                high[i] = max (high[i], point[i]);
            }
            return *this;
        }

        BoundingBox& addPoints (const Tuple<Scalar, dimensions>* points, int count) {
            for (int i = 0; i < count; ++i) {
                addPoint (points[i]);
            }
            return *this;
        }
}
