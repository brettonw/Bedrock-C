#pragma once

enum Coordinate {
    X, Y, Z, W
};

#define DOT |
#define CROSS ^
#define EXACTLY_EQUALS &
#define EXACTLY_NOT_EQUALS %

template<typename Scalar, unsigned int size>
class Tuple {
    private:
        Scalar  value[size];

        static void copy (Scalar* destination, const Scalar* source) {
            *destination++ = *source++;
            Tuple<Scalar, size - 1>::copy (destination, source);
        }

        static void add (const Scalar* left, const Scalar* right, Scalar* result) {
            *result++ = *left++ + *right++;
            Tuple<Scalar, size - 1>::add (left, right, result);
        }

        static void subtract (const Scalar* left, const Scalar* right, Scalar* result) {
            *result++ = *left++ - *right++;
            Tuple<Scalar, size - 1>::subtract (left, right, result);
        }

        static void multiply (const Scalar* left, Scalar right, Scalar* result) {
            *result++ = *left++ * right;
            Tuple<Scalar, size - 1>::multiply (left, right, result);
        }

        static void divide (const Scalar* left, Scalar right, Scalar* result) {
            *result++ = *left++ / right;
            Tuple<Scalar, size - 1>::divide (left, right, result);
        }

        static bool compare (const Scalar* left, const Scalar* right) {
            return (*left++ == *right++) ? Tuple<Scalar, size - 1>::compare (left, right) : false;
        }

    public:
        Tuple (Scalar x, ...) {
            // XXX OK, so does the compiler just put the arguments on the call stack in order?
            copy (value, &x);
        }

        Tuple (Scalar* source) {
            copy (value, source);
        }

        Tuple (const Tuple<Scalar, size>& source) {
            copy (value, source.value);
        }

        Tuple (const Tuple<Scalar, size - 1>& source, Scalar fill = 0) {
            Tuple<Scalar, size - 1>::copy (value, source.value);
            value[size - 1] = fill;
        }

        Tuple (const Tuple<Scalar, size + 1>& source) {
            // just drop the last value
            copy (value, source.value);
        }

        ~Tuple () {}

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
        static Tuple<Scalar, size>& add (const Tuple<Scalar, size>& left, const Tuple<Scalar, size>& right, Tuple<Scalar, size>& result) {
            add (left.value, right.value, result.value);
            return result;
        }

        static Tuple<Scalar, size>& subtract (const Tuple<Scalar, size>& left, const Tuple<Scalar, size>& right, Tuple<Scalar, size>& result) {
            subtract (left.value, right.value, result.value);
            return result;
        }

        static Tuple<Scalar, size>& multiply (const Tuple<Scalar, size>& left, Scalar right, Tuple<Scalar, size>& result) {
            subtract (left.value, right, result.value);
            return result;
        }

        static Tuple<Scalar, size>& divide (const Tuple<Scalar, size>& left, Scalar right, Tuple<Scalar, size>& result) {
            divide (left.value, right, result.value);
            return result;
        }

        static Tuple<Scalar, size>& negate (const Tuple<Scalar, size>& left, Tuple<Scalar, size>& result) {
            multiply (left.value, -1, result.value);
            return result;
        }

        Tuple<Scalar, size>& operator += (const Tuple<Scalar, size>& right) {
            add (value, right.value, value);
            return *this;
        }

        Tuple<Scalar, size>& operator -= (const Tuple<Scalar, size>& right) {
            subtract (value, right.value, value);
            return *this;
        }

        Tuple<Scalar, size>& operator *= (Scalar right) {
            multiply (value, right, value);
            return *this;
        }

        Tuple<Scalar, size>& operator /= (Scalar right) {
            divide (value, right, value);
            return *this;
        }

        Tuple<Scalar, size> operator + (const Tuple<Scalar, size>& right) const {
            Tuple<Scalar, size> result;
            add (value, right.value, result.value);
            return result;
        }

        Tuple<Scalar, size> operator - (const Tuple<Scalar, size>& right) const {
            Tuple<Scalar, size> result;
            subtract (value, right.value, result.value);
            return result;
        }

        Tuple<Scalar, size> operator * (Scalar right) const {
            Tuple<Scalar, size> result;
            multiply (value, right, result.value);
            return result;
        }

        Tuple<Scalar, size> operator / (Scalar right) const {
            Tuple<Scalar, size> result;
            divide (value, right, result.value);
            return result;
        }

        Tuple<Scalar, size> operator - () const {
            Tuple<Scalar, size> result;
            multiply (value, -1, result.value);
            return result;
        }

        // XXX there are three ways to do value comparison with tuples:
        // XXX 1. exact value comparisons per component
        // XXX 2. range value comparisons per component, this is effectively a discretized
        // XXX    value comparison, which checks whether the two tuples occupy a square region
        // XXX    whose size is the epsilon
        // XXX 3. subtract the vectors and take the magnitude of the delta, which checks whether
        // XXX    the two vectors occupy a circular region whose radius is the epsilon. this is
        // XXX    probably the most correct method overall, but is also the most expensive.

        bool operator ==


// ==, !=
        // norm [1, 2, mikowsky (or p), chebyshev (or max)]
        // dot, normalize, normalized, isunit
};

template<typename Scalar, unsigned int size>
Tuple<Scalar, size> operator * (Scalar left, const Tuple<Scalar, size>* right) {
    return right * left;
}

template<typename Scalar>
class Tuple<Scalar, 0> {
    private:
        static void copy (Scalar* destination, const Scalar* source) {}
        static void add (const Scalar* left, const Scalar* right, Scalar* result) {}
        static void subtract (const Scalar* left, const Scalar* right, Scalar* result) {}
        static void multiply (const Scalar* left, Scalar right, Scalar* result) {}
        static void divide (const Scalar* left, Scalar right, Scalar* result) {}
        static bool compare (const Scalar* left, const Scalar* right) { return true; }
};

