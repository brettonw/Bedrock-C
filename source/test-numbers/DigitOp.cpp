#include "Test.h"
#include "DigitOp.h"

/*
    // basic operations on a single array of digits
    static uint top (const DigitType* a) { return 0; }
    static void set (DigitType* dst, DigitType src) {}
    static bool test (const DigitType* a) { return false; }

    // basic operations on two arrays of digits
    static void copy (DigitType* dst, const DigitType* src) {}
    static int compare (const DigitType* a, const DigitType* b) { return 0; }

    // operations with an array of digits and a single digit
    static DigitType digitShiftLeft (const DigitType* a, DigitType* result, DigitType fill = 0) { return fill; }
    static DigitType digitShiftRight (const DigitType* a, DigitType* result, DigitType fill = 0) { return fill; }
    static int compareDigit (const DigitType* a, DigitType b) { return signum (a[0] - b); }

    // bit operations on a single array of digits
    static void bitAnd (const DigitType* a, const DigitType* b, DigitType* result) {}
    static void bitOr (const DigitType* a, const DigitType* b, DigitType* result) {}
    static void bitXor (const DigitType* a, const DigitType* b, DigitType* result) {}
    static void bitComplement (const DigitType* a, DigitType* result) {}
*/
TEST_CASE(TestDigitOpTop) {
    Log::Scope scope (Log::DEBUG);
    u4 testValue;
    uint top;

    testValue = 0;
    top = DigitOp<u1, 4>::top (reinterpret_cast<u1*>(&testValue));
    TEST_XY(top, 0);

    testValue = 0x01000000;
    top = DigitOp<u1, 4>::top (reinterpret_cast<u1*>(&testValue));
    TEST_XY(top, 1);

    testValue = 0x00010000;
    top = DigitOp<u1, 4>::top (reinterpret_cast<u1*>(&testValue));
    TEST_XY(top, 2);

    testValue = 0x00000100;
    top = DigitOp<u1, 4>::top (reinterpret_cast<u1*>(&testValue));
    TEST_XY(top, 3);

    testValue = 0x00000001;
    top = DigitOp<u1, 4>::top (reinterpret_cast<u1*>(&testValue));
    TEST_XY(top, 4);
}

TEST_CASE(TestDigitOpSet) {
    Log::Scope scope (Log::DEBUG);
    u4 testValue;
    //top = DigitOp<u1, 4>::top (&testValue);

    TEST_X(true);
}
