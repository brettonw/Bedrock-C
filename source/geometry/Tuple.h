#pragma once

#include "TupleBase.h"

// set up a basic tuple interface
template<typename Scalar, unsigned int size>
class Tuple : public TupleBase<Scalar, size, Tuple<Scalar, size> > {
        typedef TupleBase<Scalar, size, Tuple<Scalar, size> > BaseType;
    public:
        using BaseType::TupleBase;
};

// some tuples add a cross product, in 2D it's the magnitude of the vertical vector that would
// result from a 3D operation if the Z components were 0. In 3D, it's a vector that is
// perpendicular to the two provided vectors
// see: https://en.wikipedia.org/wiki/Cross_product

template<typename Scalar>
class Tuple2 : public TupleBase<Scalar, 2, Tuple2<Scalar> > {
        typedef TupleBase<Scalar, 2, Tuple2<Scalar> > BaseType;
    public:
        // expose the empty constructor, a basic 2-Scalar version, and all the Tuple constructors
        Tuple2 () : BaseType () {}
        Tuple2 (Scalar x, Scalar y) {
            // parameters go on the stack in reverse order
            TupleHelper<Scalar, 2>::rcopy (BaseType::value, &x);
        }
        using BaseType::TupleBase;

        Scalar operator CROSS (const Tuple2<Scalar>& right) const {
            return ((*this)[X] * right[Y]) - ((*this)[Y] * right[X]);
        }

        Tuple2<Scalar> perpendicular () const {
            return Tuple2<Scalar> ((*this)[Y], -(*this)[X]);
        }
};

template<typename Scalar>
class Tuple3 : public TupleBase<Scalar, 3, Tuple3<Scalar> > {
        typedef TupleBase<Scalar, 3, Tuple3<Scalar> > BaseType;
    public:
        // expose the empty constructor, a basic 3-Scalar version, and all the Tuple constructors
        Tuple3 () : BaseType () {}
        Tuple3 (Scalar x, Scalar y, Scalar z) {
            // parameters go on the stack in reverse order
            TupleHelper<Scalar, 3>::rcopy (BaseType::value, &x);
        }
        using BaseType::TupleBase;

        Tuple3<Scalar> operator CROSS (const Tuple3<Scalar>& right) const {
            return Tuple3<Scalar> ((BaseType::value[Y] * right[Z]) - (BaseType::value[Z] * right[Y]),
                                   (BaseType::value[Z] * right[X]) - (BaseType::value[X] * right[Z]),
                                   (BaseType::value[X] * right[Y]) - (BaseType::value[Y] * right[X]));
        }
};

template<typename Scalar>
class Tuple4 : public TupleBase<Scalar, 4, Tuple4<Scalar> > {
        typedef TupleBase<Scalar, 4, Tuple4<Scalar> > BaseType;
    public:
        // expose the empty constructor, a basic 4-Scalar version, and all the Tuple constructors
        Tuple4 () : BaseType () {}
        Tuple4 (Scalar x, Scalar y, Scalar z, Scalar w) {
            // parameters go on the stack in reverse order
            TupleHelper<Scalar, 4>::rcopy (BaseType::value, &x);
        }
        using BaseType::TupleBase;
};

// computer graphics topics often use a homogenous coordinate space (x, y, z, w), where the w
// component is 0 for a direction, and 1 for a point location. this enables a 4x4 transformation
// matrix that includes translation, and perspective projections, where otherwise only rotation is
// possible

// and last - just make some actual simple names for the most commonly used types
typedef Tuple2<f8> Vector2;
typedef Tuple3<f8> Vector3;
typedef Tuple4<f8> Vector4;
