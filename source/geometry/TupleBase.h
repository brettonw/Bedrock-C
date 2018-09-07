#pragma once

#include "TupleHelper.h"

enum Coordinate {
    X = 0, Y = 1, Z = 2, W = 3,
    U = 0, V = 1
};

#define DOT |
#define CROSS ^

template<typename Scalar, unsigned int size, typename DerivedType>
class TupleBase {
        typedef TupleBase<Scalar, size, DerivedType> BaseType;
    protected:
        Scalar  value[size];
        static Scalar epsilon;

    public:
        static const BaseType ZERO;

        TupleBase () {
            TupleHelper<Scalar, size>::fill(value, 0);
        }

        TupleBase (Scalar fillValue) {
            TupleHelper<Scalar, size>::fill(value, fillValue);
        }

        TupleBase (const initializer_list<Scalar>& initializers) {
            // XXX OK, so does the compiler just put the arguments on the call stack in order?
            TupleHelper<Scalar, size>::copy (value, initializers.begin ());
        }

        TupleBase (Scalar* source) {
            TupleHelper<Scalar, size>::copy (value, source);
        }

        TupleBase (const BaseType& source) {
            TupleHelper<Scalar, size>::copy (value, source.value);
        }

        template<typename OtherDerivedType>
        TupleBase (const TupleBase<Scalar, size - 1, OtherDerivedType>& source, Scalar fill = 0) {
            TupleHelper<Scalar, size - 1>::copy (value, *source);
            value[size - 1] = fill;
        }

        template<typename OtherDerivedType>
        TupleBase (const TupleBase<Scalar, size + 1, OtherDerivedType>& source) {
            // just drop the last value
            TupleHelper<Scalar, size>::copy (value, *source);
        }

        // don't ever add a destructor...
        //~TupleBase () {}

        // accessors
        Scalar& operator [] (Coordinate index) {
            return value[index];
        }

        const Scalar& operator [] (Coordinate index) const {
            return value[index];
        }

        Scalar* operator * () {
            return value;
        }

        const Scalar* operator * () const {
            return value;
        }

        // add/subtract, multiply/divide by scalar, negate
        static BaseType& add (const BaseType& left, const BaseType& right, BaseType& result) {
            TupleHelper<Scalar, size>::add (left.value, right.value, result.value);
            return result;
        }

        static BaseType& subtract (const BaseType& left, const BaseType& right, BaseType& result) {
            TupleHelper<Scalar, size>::subtract (left.value, right.value, result.value);
            return result;
        }

        static BaseType& multiply (const BaseType& left, Scalar right, BaseType& result) {
            TupleHelper<Scalar, size>::multiply (left.value, right, result.value);
            return result;
        }

        static BaseType& divide (const BaseType& left, Scalar right, BaseType& result) {
            TupleHelper<Scalar, size>::divide (left.value, right, result.value);
            return result;
        }

        static BaseType& negate (const BaseType& left, BaseType& result) {
            TupleHelper<Scalar, size>::multiply (left.value, -1, result.value);
            return result;
        }

        DerivedType& operator += (const BaseType& right) {
            return static_cast<DerivedType&> (add (*this, right, *this));
        }

        DerivedType& operator -= (const BaseType& right) {
            return static_cast<DerivedType&> (subtract (*this, right, *this));
        }

        DerivedType& operator *= (Scalar right) {
            return static_cast<DerivedType&> (multiply (*this, right, *this));
        }

        DerivedType& operator /= (Scalar right) {
            return static_cast<DerivedType&> (divide (*this, right, *this));
        }

        DerivedType operator + (const BaseType& right) const {
            DerivedType result;
            return static_cast<DerivedType&> (add (*this, right, result));
        }

        DerivedType operator - (const BaseType& right) const {
            DerivedType result;
            return static_cast<DerivedType&> (subtract (*this, right, result));
        }

        DerivedType operator * (Scalar right) const {
            DerivedType result;
            return static_cast<DerivedType&> (multiply (*this, right, result));
        }

        DerivedType operator / (Scalar right) const {
            DerivedType result;
            return static_cast<DerivedType&> (divide (*this, right, result));
        }

        DerivedType operator - () const {
            DerivedType result;
            return static_cast<DerivedType&> (multiply (*this, -1, result));
        }

        // norms, measures, lengths
        // norm L-1, a.k.a. Manhattan distance
        Scalar normL1 () const {
            return TupleHelper<Scalar, size>::sumAbs (value);
        }

        // norm L-2, a.k.a. Euclidean distance, or length
        // this is what most people think of when they use a norm as a unit of measure, so we
        // include the expected names in multiple implementations
        Scalar normL2 () const {
            return sqrt (TupleHelper<Scalar, size>::sumSquare (value));
        }
        Scalar norm () const {
            return normL2 ();
        }
        Scalar length () const {
            return normL2 ();
        }

        // the infinity norm, a.k.a. max norm
        Scalar normInf () const {
            return TupleHelper<Scalar, size>::max (value);
        }

        // the power norm general function, a.k.a. p-norm
        // see: https://en.wikipedia.org/wiki/Norm_(mathematics)
        Scalar normPower (double power) const {
            return pow (TupleHelper<Scalar, size>::sumPower (value, power), 1.0 / power);
        }

        // normalization
        static BaseType& normalize (const BaseType& source, BaseType& result) {
            return divide (source, source.norm (), result);
        }

        DerivedType& normalize () {
            return static_cast<DerivedType&> (normalize (*this, *this));
        }

        DerivedType normalized () const {
            DerivedType result;
            return static_cast<DerivedType&> (normalize (*this, result));
        }

        bool isUnit () const {
            return (abs (norm () - 1.0) <= epsilon);
        }

        // comparisons
        // there are three ways to do value comparison with tuples:
        // 1. exact value comparisons per component
        // 2. range value comparisons per component, this is effectively a discretized
        //    value comparison, which checks whether the two tuples occupy a square region
        //    whose size is the epsilon (option 1 is a simplified version of this option where
        //    epsilon = 0)
        // 3. subtract the vectors and take the magnitude of the delta, which checks whether
        //    the two vectors occupy a circular region whose radius is the epsilon. this is
        //    probably the most correct method overall, but is also the most expensive.
        // we use #3, on the theory that the cost is acceptable if you actually require this check.

        bool operator == (const BaseType& right) const {
            BaseType delta;
            return (subtract (*this, right, delta).norm () <= epsilon);
        }

        bool operator != (const BaseType& right) const {
            return not operator == (right);
        }

        // epsilon
        static Scalar getEpsilon () {
            return epsilon;
        }

        static void setEpsilon (Scalar eps) {
            epsilon = eps;
        }

        // a scope-based helper for setting epsilon
        class Scope {
            private:
                Scalar oldEpsilon;

            public:
                Scope () : oldEpsilon (BaseType::epsilon) { }

                Scope (Scalar newEpsilon) : oldEpsilon (BaseType::epsilon) {
                   BaseType::epsilon = newEpsilon;
                }

                ~Scope () {
                    BaseType::epsilon = oldEpsilon;
                 }
        };


        // dot
        Scalar operator DOT (const BaseType& right) const {
            return TupleHelper<Scalar, size>::sumMult (value, right.value);
        }
};

template<typename Scalar, unsigned int size, typename DerivedType>
Scalar TupleBase<Scalar, size, DerivedType>::epsilon = numeric_limits<Scalar>::epsilon();

template<typename Scalar, unsigned int size, typename DerivedType>
const TupleBase<Scalar, size, DerivedType> TupleBase<Scalar, size, DerivedType>::ZERO (0.0);

// tuple multiplication is commutative, so we implement (left * right) by calling (right * left)
template<typename LeftType, typename Scalar, unsigned int size, typename DerivedType>
DerivedType operator * (LeftType left, const TupleBase<Scalar, size, DerivedType>& right) {
    return right * Scalar (left);
}

// ostream writer...
template<typename Scalar, unsigned int size, typename DerivedType>
ostream& operator << (ostream& stream, const TupleBase<Scalar, size, DerivedType>& tuple) {
    const char* spacer = "(";
    for (unsigned int i = 0; i < size; ++i) {
        stream << spacer << tuple[static_cast<Coordinate> (i)];
        spacer = ", ";
    }
    return stream << ")";
}
