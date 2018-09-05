#pragma once

#include "TupleBase.h"

// set up a basic tuple interface
template<typename Scalar, unsigned int size>
class Tuple : public TupleBase<Scalar, size, Tuple<Scalar, size> > {
        typedef TupleBase<Scalar, size, Tuple<Scalar, size> > BaseClass;
    public:
        using BaseClass::TupleBase;
};

// some tuples add a cross product, in 2D it's the magnitude of the vertical vector that would
// result from a 3D operation if the Z components were 0. In 3D, it's a vector that is
// perpendicular to the two provided vectors
// see: https://en.wikipedia.org/wiki/Cross_product

template<typename Scalar>
class Tuple2 : public TupleBase<Scalar, 2, Tuple2<Scalar> > {
    typedef TupleBase<Scalar, 2, Tuple2<Scalar> > BaseClass;
    public:
        using BaseClass::TupleBase;

        Scalar operator CROSS (const Tuple2<Scalar>& right) const {
            return ((*this)[X] * right[Y]) - ((*this)[Y] * right[X]);
        }

        Tuple2<Scalar> perpendicular () const {
            return Tuple2<Scalar> ((*this)[Y], -(*this)[X]);
        }
};

template<typename Scalar>
class Tuple3 : public TupleBase<Scalar, 3, Tuple3<Scalar> > {
    typedef TupleBase<Scalar, 3, Tuple3<Scalar> > BaseClass;
    public:
        using BaseClass::TupleBase;

        Tuple3<Scalar> operator CROSS (const Tuple3<Scalar>& right) const {
            // XXX TODO
            return Tuple3<Scalar> (1);
        }
};

// computer graphics topics often use a homogenous coordinate space (x, y, z, w), where the w
// component is 0 for a direction, and 1 for a point location. this enables a 4x4 transformation
// matrix that includes translation, and perspective projections, wherre otherwise only rotation is
// possible
