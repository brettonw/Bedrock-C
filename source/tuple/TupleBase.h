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
    protected:
        Scalar  value[size];
        static Scalar epsilon;

    public:
        static const TupleBase<Scalar, size, DerivedType> ZERO;

        TupleBase (Scalar fillValue = 0) {
            TupleHelper<Scalar, size>::fill(value, fillValue);
        }

        TupleBase (const initializer_list<Scalar>& initializers) {
            // XXX OK, so does the compiler just put the arguments on the call stack in order?
            TupleHelper<Scalar, size>::copy (value, initializers.begin ());
        }

        TupleBase (Scalar* source) {
            TupleHelper<Scalar, size>::copy (value, source);
        }

        TupleBase (const TupleBase<Scalar, size, DerivedType>& source) {
            TupleHelper<Scalar, size>::copy (value, source.value);
        }

        TupleBase (const TupleBase<Scalar, size - 1, DerivedType>& source, Scalar fill = 0) {
            TupleHelper<Scalar, size - 1>::copy (value, source.value);
            value[size - 1] = fill;
        }

        TupleBase (const TupleBase<Scalar, size + 1, DerivedType>& source) {
            // just drop the last value
            TupleHelper<Scalar, size>::copy (value, source.value);
        }

        // don't ever add a destructor...
        //~TupleBase () {}

        // assignment
        TupleBase<Scalar, size, DerivedType>& operator = (const TupleBase<Scalar, size, DerivedType>& source) {
            TupleHelper<Scalar, size>::copy (value, source.value);
        }

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
        static TupleBase<Scalar, size, DerivedType>& add (const TupleBase<Scalar, size, DerivedType>& left, const TupleBase<Scalar, size, DerivedType>& right, TupleBase<Scalar, size, DerivedType>& result) {
            TupleHelper<Scalar, size>::add (left.value, right.value, result.value);
            return result;
        }

        static TupleBase<Scalar, size, DerivedType>& subtract (const TupleBase<Scalar, size, DerivedType>& left, const TupleBase<Scalar, size, DerivedType>& right, TupleBase<Scalar, size, DerivedType>& result) {
            TupleHelper<Scalar, size>::subtract (left.value, right.value, result.value);
            return result;
        }

        static TupleBase<Scalar, size, DerivedType>& multiply (const TupleBase<Scalar, size, DerivedType>& left, Scalar right, TupleBase<Scalar, size, DerivedType>& result) {
            TupleHelper<Scalar, size>::multiply (left.value, right, result.value);
            return result;
        }

        static TupleBase<Scalar, size, DerivedType>& divide (const TupleBase<Scalar, size, DerivedType>& left, Scalar right, TupleBase<Scalar, size, DerivedType>& result) {
            TupleHelper<Scalar, size>::divide (left.value, right, result.value);
            return result;
        }

        static TupleBase<Scalar, size, DerivedType>& negate (const TupleBase<Scalar, size, DerivedType>& left, TupleBase<Scalar, size, DerivedType>& result) {
            TupleHelper<Scalar, size>::multiply (left.value, -1, result.value);
            return result;
        }

        TupleBase<Scalar, size, DerivedType>& operator += (const TupleBase<Scalar, size, DerivedType>& right) {
            add (value, right.value, value);
            return *this;
        }

        TupleBase<Scalar, size, DerivedType>& operator -= (const TupleBase<Scalar, size, DerivedType>& right) {
            return subtract (*this, right, *this);
        }

        TupleBase<Scalar, size, DerivedType>& operator *= (Scalar right) {
            return multiply (*this, right, *this);
        }

        TupleBase<Scalar, size, DerivedType>& operator /= (Scalar right) {
            return divide (*this, right, *this);
        }

        TupleBase<Scalar, size, DerivedType> operator + (const TupleBase<Scalar, size, DerivedType>& right) const {
            TupleBase<Scalar, size, DerivedType> result;
            return add (*this, right, result);
        }

        TupleBase<Scalar, size, DerivedType> operator - (const TupleBase<Scalar, size, DerivedType>& right) const {
            TupleBase<Scalar, size, DerivedType> result;
            return subtract (*this, right, result);
        }

        TupleBase<Scalar, size, DerivedType> operator * (Scalar right) const {
            TupleBase<Scalar, size, DerivedType> result;
            return multiply (*this, right, result);
        }

        TupleBase<Scalar, size, DerivedType> operator / (Scalar right) const {
            TupleBase<Scalar, size, DerivedType> result;
            return divide (*this, right, result);
        }

        TupleBase<Scalar, size, DerivedType> operator - () const {
            TupleBase<Scalar, size, DerivedType> result;
            return multiply (*this, -1, result);
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
        static TupleBase<Scalar, size, DerivedType> normalized (const TupleBase<Scalar, size, DerivedType>& source, TupleBase<Scalar, size, DerivedType>& result) {
            return divide (source, source.norm (), result);
        }

        TupleBase<Scalar, size, DerivedType>& normalize () {
            return normalized (*this, *this);
        }

        TupleBase<Scalar, size, DerivedType> normalized () const {
            TupleBase<Scalar, size, DerivedType> result;
            return normalized (*this, result);
        }

        static TupleBase<Scalar, size, DerivedType> unit (const initializer_list<Scalar>& initializers) {
            return TupleBase<Scalar, size, DerivedType> (initializers).normalized();
        }

        bool isUnit () const {
            return (abs (norm () - 1.0) <= epsilon);
        }

        // comparisons
        // XXX there are three ways to do value comparison with tuples:
        // XXX 1. exact value comparisons per component
        // XXX 2. range value comparisons per component, this is effectively a discretized
        // XXX    value comparison, which checks whether the two tuples occupy a square region
        // XXX    whose size is the epsilon (option 1 is a simplified version of this option where
        // XXX    epsilon = 0)
        // XXX 3. subtract the vectors and take the magnitude of the delta, which checks whether
        // XXX    the two vectors occupy a circular region whose radius is the epsilon. this is
        // XXX    probably the most correct method overall, but is also the most expensive.

        bool operator == (const TupleBase<Scalar, size, DerivedType>& right) const {
            TupleBase<Scalar, size, DerivedType> delta;
            return (subtract (*this, right, delta).norm () <= epsilon);
        }

        bool operator != (const TupleBase<Scalar, size, DerivedType>& right) const {
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
                Scope () : oldEpsilon (TupleBase<Scalar, size, DerivedType>::epsilon) { }

                Scope (Scalar newEpsilon) : oldEpsilon (TupleBase<Scalar, size, DerivedType>::epsilon) {
                   TupleBase<Scalar, size, DerivedType>::epsilon = newEpsilon;
                }

                ~Scope () {
                    TupleBase<Scalar, size, DerivedType>::epsilon = oldEpsilon;
                 }
        };


        // dot
        Scalar operator DOT (const TupleBase<Scalar, size, DerivedType>& right) const {
            return TupleHelper<Scalar, size>::sumMult (*this, right);
        }
};

template<typename Scalar, unsigned int size, typename DerivedType>
Scalar TupleBase<Scalar, size, DerivedType>::epsilon = numeric_limits<Scalar>::epsilon();

template<typename Scalar, unsigned int size, typename DerivedType>
const TupleBase<Scalar, size, DerivedType> TupleBase<Scalar, size, DerivedType>::ZERO (0.0);

// tuple multiplication is commutative, so we implement (left * right) by calling (right * left)
template<typename Scalar, unsigned int size, typename DerivedType>
TupleBase<Scalar, size, DerivedType> operator * (Scalar left, const TupleBase<Scalar, size, DerivedType>* right) {
    return right * left;
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
