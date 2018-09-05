#pragma once

#include "Common.h"

template<typename Scalar, unsigned int size>
class TupleHelper {
    public:
        static void fill (Scalar* destination, Scalar fillValue) {
            *destination++ = fillValue;
            TupleHelper<Scalar, size - 1>::fill (destination, fillValue);
        }

        static void copy (Scalar* destination, const Scalar* source) {
            *destination++ = *source++;
            TupleHelper<Scalar, size - 1>::copy (destination, source);
        }

        static void add (const Scalar* left, const Scalar* right, Scalar* result) {
            *result++ = *left++ + *right++;
            TupleHelper<Scalar, size - 1>::add (left, right, result);
        }

        static void subtract (const Scalar* left, const Scalar* right, Scalar* result) {
            *result++ = *left++ - *right++;
            TupleHelper<Scalar, size - 1>::subtract (left, right, result);
        }

        static void multiply (const Scalar* left, Scalar right, Scalar* result) {
            *result++ = *left++ * right;
            TupleHelper<Scalar, size - 1>::multiply (left, right, result);
        }

        static void divide (const Scalar* left, Scalar right, Scalar* result) {
            *result++ = *left++ / right;
            TupleHelper<Scalar, size - 1>::divide (left, right, result);
        }

        static bool compare (const Scalar* left, const Scalar* right) {
            return (*left++ == *right++) ? TupleHelper<Scalar, size - 1>::compare (left, right) : false;
        }

        static Scalar sumAbs (const Scalar* source) {
            return abs (*source++) + TupleHelper<Scalar, size - 1>::sumAbs (source);
        }

        static Scalar sumSquare (const Scalar* source) {
            Scalar value = *source++;
            return (value * value) + TupleHelper<Scalar, size - 1>::sumSquare (source);
        }

        static Scalar sumMult (const Scalar* left, const Scalar* right) {
            return (*left++ * *right++) + TupleHelper<Scalar, size - 1>::sumMult (left, right);
        }

        static Scalar sumPower (const Scalar* source, Scalar power) {
            return pow (*source++, power) + TupleHelper<Scalar, size - 1>::sumPower (source);
        }

        static Scalar max (const Scalar* source) {
            return max (*source++,  TupleHelper<Scalar, size - 1>::max (source));
        }
};

template<typename Scalar>
class TupleHelper<Scalar, 0> {
    public:
        static void fill (Scalar* destination, Scalar fillValue) {}
        static void copy (Scalar* destination, const Scalar* source) {}
        static void add (const Scalar* left, const Scalar* right, Scalar* result) {}
        static void subtract (const Scalar* left, const Scalar* right, Scalar* result) {}
        static void multiply (const Scalar* left, Scalar right, Scalar* result) {}
        static void divide (const Scalar* left, Scalar right, Scalar* result) {}
        static bool compare (const Scalar* left, const Scalar* right) { return true; }
        static Scalar sumAbs (const Scalar* source) { return 0; }
        static Scalar sumSquare (const Scalar* source) { return 0; }
        static Scalar sumMult (const Scalar* left, const Scalar* right) { return 0; }
        static double sumPower (const Scalar* source, Scalar power) { return 0; }
        static Scalar max (const Scalar* source) { return numeric_limits<Scalar>::lowest (); }
};

