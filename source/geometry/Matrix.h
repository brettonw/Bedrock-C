#pragma once

#include "MatrixHelper.h"

template<typename Scalar, uint m, uint n>
class Matrix {
    public:
        typedef Matrix<Scalar, m, n> Mat;

    private:
        Scalar  nm[m * n];

    public:
        Matrix () {}
        Matrix (const Mat& matrix) {
            TupleHelper<Scalar, m * n>::copy(nm, matrix.nm);
        }

        Scalar& operator () (uint row, uint column) {
            return nm[column + (row * m)];
        }

        template <int dim = m, typename std::enable_if<dim == n, void>::type* = nullptr>
        Matrix& identity () {
            IdentityHelper<Scalar, m, m, m>::identity(nm);
            return *this;
        }

        // fill
        // identity
        // transpose
        // invert

        // operators
        // multiply
        // scalar multiplication
        // add
};


// 2d and 3d matrices
// scale, translate, rotate<Axis>
// frustum viewmatrix, lookfrom, etc.
