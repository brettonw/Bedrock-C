#pragma once

#include "TupleHelper.h"


template<typename Scalar, uint n, uint m, uint size>
class IdentityHelper {
    public:
        // only defined for square matrices
        static void identity (Scalar* destination) {
            *destination = (m == n) ? 1.0 : 0.0;
            IdentityHelper<Scalar, n, m - 1, size>::identity (destination + 1);
        }
};

template<typename Scalar, uint n, uint size>
class IdentityHelper<Scalar, n, 0, size> {
    public:
        // only defined for square matrices
        static void identity (Scalar* destination) {
            IdentityHelper<Scalar, n - 1, size, size>::identity (destination);
        }
};

template<typename Scalar, uint size>
class IdentityHelper<Scalar, 0, 0, size> {
    public:
        // only defined for square matrices
        static void identity (Scalar* destination) {}
};


