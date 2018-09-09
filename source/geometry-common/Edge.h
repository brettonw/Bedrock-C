#pragma once

#include "Tuple.h"

template<typename Scalar, unsigned int dimension>
struct Edge {
    Tuple<Scalar, dimension> a;
    Tuple<Scalar, dimension> b;

    Edge (void) {}
    Edge (const Tuple<Scalar, dimension>& _a, const Tuple<Scalar, dimension>& _b) : a (_a), b (_b) {}
};

