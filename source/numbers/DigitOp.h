#pragma once

#include "Common.h"

// DigitOp is a helper class for working with arrays of "digits", as part of an implementation of a
// big-integer class. arrays of digits are organized in Least-Significant-Digit first fashion, so
// digit 0 is the LSD (aka little endian)

#define Q ((N - M) - 1)

template<typename DigitType, int N, int M = 0>
struct DigitOp {
    // basic operations on a single array of digits

    // return the index of the first non-zero digit in the number, where 0 means no digits are
    // non-zero. this is used extensively in framing the calculations between two numbers
    static uint top (const DigitType* a) {
        return a[Q] ? M : DigitOp<DigitType, N, M + 1>::top (a);
    }

    // set all the digits of an array to a source value
    static void set (DigitType* dst, DigitType src) {
        dst[M] = src;
        DigitOp<DigitType, N, M + 1>::set (dst, src);
    }

    // return true if any digit in the array is non-zero
    static bool test (const DigitType* a) {
        return a[M] ? true : DigitOp<DigitType, N, M + 1>::test (a);
    }

    // basic operations on two arrays of digits

    // copy one digit array to another
    static void copy (DigitType* dst, const DigitType* src) {
        *dst++ = *src++;
        DigitOp<DigitType, N, M + 1>::copy (dst, src);
    }

    // compare two digit arrays, returning a signum (-1, 0, 1) if a <, == , or > b, respectively
    static int compare (const DigitType* a, const DigitType* b) {
        return (a[Q] > b[Q]) ? 1 : ((a[Q] < b[Q]) ? -1 : DigitOp<DigitType, N, M + 1>::compare (a, b));
    }

    // operations with an array of digits and a single digit

    // roll the entire array one digit to the left, returns the rolled out digit - safe to shift a
    // digit array onto itself
    static DigitType rotateLeft (const DigitType* a, DigitType* result, DigitType fill = 0) {
        DigitType temp = a[M];
        result[M] = fill;
        return DigitOp<DigitType, N, M + 1>::digitShiftLeft (a, result, temp);
    }

    // roll the entire array one digit to the right, returns the rolled out digit - safe to shift a
    // digit array onto itself
    static DigitType rotateRight (const DigitType* a, DigitType* result, DigitType fill = 0) {
        DigitType temp = a[Q];
        result[Q] = fill;
        return DigitOp<DigitType, N, M + 1>::digitShiftRight (a, result, temp);
    }

    // compare a digit array against a single digit, returning a signum (-1, 0, 1) if a <, == , or
    // > b, respectively. it is safe to shift a digit array onto itself
    static int compareDigit (const DigitType* a, DigitType b) {
        return a[Q] ? 1 : DigitOp<DigitType, N, M +1>::compareDigit (a, b);
    }

    static void bitAnd (const DigitType* a, const DigitType* b, DigitType* result) {
        result[M] = a[M] bit_and b[M];
        DigitOp<DigitType, N, M + 1>::bitAnd (a, b, result);
    }

    static void bitOr (const DigitType* a, const DigitType* b, DigitType* result) {
        result[M] = a[M] bit_or b[M];
        DigitOp<DigitType, N, M + 1>::bitOr (a, b, result);
    }

    static void bitXor (const DigitType* a, const DigitType* b, DigitType* result) {
        result[M] = a[M] xor b[M];
        DigitOp<DigitType, N, M + 1>::bitXor (a, b, result);
    }

    static void bitComplement (const DigitType* a, DigitType* result) {
        result[M] = complement a[M];
        DigitOp<DigitType, N, M + 1>::bitComplement (a, result);
    }
};

// specialized for N = 0
template<typename DigitType, int N>
struct DigitOp<DigitType, N, N> {
    // basic operations on a single array of digits
    static uint top (const DigitType* a) { return N; }
    static void set (DigitType* dst, DigitType src) {}
    static bool test (const DigitType* a) { return false; }

    // basic operations on two arrays of digits
    static void copy (DigitType* dst, const DigitType* src) {}
    static int compare (const DigitType* a, const DigitType* b) { return 0; }

    // operations with an array of digits and a single digit
    static DigitType rotateLeft (const DigitType* a, DigitType* result, DigitType fill = 0) { return fill; }
    static DigitType rotateRight (const DigitType* a, DigitType* result, DigitType fill = 0) { return fill; }
    static int compareDigit (const DigitType* a, DigitType b) { return  (a[0] < b) ? -1 : ((a[0] > b) ? 1 : 0); }

    // bit operations on a single array of digits
    static void bitAnd (const DigitType* a, const DigitType* b, DigitType* result) {}
    static void bitOr (const DigitType* a, const DigitType* b, DigitType* result) {}
    static void bitXor (const DigitType* a, const DigitType* b, DigitType* result) {}
    static void bitComplement (const DigitType* a, DigitType* result) {}
};

