#include "Test.h"
#include "Misb.h"

TEST_CASE(MisbComputeByteBount) {
    //void Misb::computeByteCount (double min, double max, double precision, uint* byteCount)
    uint byteCount = 0;
    Misb::computeByteCount (0, 1, 0.1, &byteCount);
    TEST_EQUALS(byteCount, 1);
    Misb::computeByteCount (0, 1, 0.01, &byteCount);
    TEST_EQUALS(byteCount, 1);
    Misb::computeByteCount (0, 1, 0.001, &byteCount);
    TEST_EQUALS(byteCount, 2);
    Misb::computeByteCount (0, 1, 0.0001, &byteCount);
    TEST_EQUALS(byteCount, 2);
    Misb::computeByteCount (0, 1, 0.00001, &byteCount);
    TEST_EQUALS(byteCount, 3);
    Misb::computeByteCount (0, 1, 0.000001, &byteCount);
    TEST_EQUALS(byteCount, 3);
    Misb::computeByteCount (0, 1, 0.0000001, &byteCount);
    TEST_EQUALS(byteCount, 4);

    Misb::computeByteCount (-1, 1, 0.1, &byteCount);
    TEST_EQUALS(byteCount, 1);
    Misb::computeByteCount (-1, 1, 0.01, &byteCount);
    TEST_EQUALS(byteCount, 2);
    Misb::computeByteCount (-1, 1, 0.001, &byteCount);
    TEST_EQUALS(byteCount, 2);
    Misb::computeByteCount (-1, 1, 0.0001, &byteCount);
    TEST_EQUALS(byteCount, 2);
    Misb::computeByteCount (-1, 1, 0.00001, &byteCount);
    TEST_EQUALS(byteCount, 3);
    Misb::computeByteCount (-1, 1, 0.000001, &byteCount);
    TEST_EQUALS(byteCount, 3);
    Misb::computeByteCount (-1, 1, 0.0000001, &byteCount);
    TEST_EQUALS(byteCount, 4);
}

TEST_CASE(MisbComputeScalesAndOffset) {
    // void computeScalesAndOffset (double min, double max, int byteCount, double* encodeScale, double* decodeScale, double* offset);
    double encodeScale, decodeScale, offset;

    Log::debug () << "Scales 0..1" << endl;
    Misb::computeScalesAndOffset (0, 1, 1, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 7));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 7));
    TEST_EQUALS(offset, 0);
    Misb::computeScalesAndOffset (0, 1, 2, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 15));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 15));
    TEST_EQUALS(offset, 0);
    Misb::computeScalesAndOffset (0, 1, 3, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 23));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 23));
    TEST_EQUALS(offset, 0);
    Misb::computeScalesAndOffset (0, 1, 4, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1L << 31));
    TEST_EQUALS(decodeScale, 1.0 / (1L << 31));
    TEST_EQUALS(offset, 0);

    Log::debug () << "Scales -1..1" << endl;
    Misb::computeScalesAndOffset (-1, 1, 1, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 6));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 6));
    TEST_EQUALS(offset, 0);
    Misb::computeScalesAndOffset (-1, 1, 2, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 14));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 14));
    TEST_EQUALS(offset, 0);
    Misb::computeScalesAndOffset (-1, 1, 3, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 22));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 22));
    TEST_EQUALS(offset, 0);
    Misb::computeScalesAndOffset (-1, 1, 4, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1L << 30));
    TEST_EQUALS(decodeScale, 1.0 / (1L << 30));
    TEST_EQUALS(offset, 0);

    Log::debug () << "Scales -0.33..1" << endl;
    Misb::computeScalesAndOffset (-0.33, 1, 1, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 6));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 6));
    TEST_EQUALS_EPS(offset, 0.88, 0.01);
    Misb::computeScalesAndOffset (-0.33, 1, 2, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 14));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 14));
    TEST_EQUALS_EPS(offset, 0.28, 0.01);
    Misb::computeScalesAndOffset (-0.33, 1, 3, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 22));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 22));
    TEST_EQUALS_EPS(offset, 0.68, 0.01);
    Misb::computeScalesAndOffset (-0.33, 1, 4, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1L << 30));
    TEST_EQUALS(decodeScale, 1.0 / (1L << 30));
    TEST_EQUALS_EPS(offset, 0.08, 0.01);
}

TEST_CASE(MisbEncodeDecode) {

    // test the obvious in the range 0..1
    // test in the range -1..1
    // test in the range 1000..2000
    double testFloat = 0.33;
    TEST_EQUALS(Misb::encode (testFloat, 0, 1, 0.1), 0x2a);
    TEST_EQUALS(Misb::encode (testFloat, 0, 1, 0.01), 0x2a);
    TEST_EQUALS(Misb::encode (testFloat, 0, 1, 0.001), 0x2a3d);
    TEST_EQUALS(Misb::encode (testFloat, 0, 1, 0.0001), 0x2a3d);
    TEST_EQUALS(Misb::encode (testFloat, 0, 1, 0.00001), 0x2a3d70);
    TEST_EQUALS(Misb::encode (testFloat, 0, 1, 0.000001), 0x2a3d70);

    TEST_EQUALS_EPS(Misb::decode (0x2a, 0, 1, 0.1), testFloat, 0.1);
    TEST_EQUALS_EPS(Misb::decode (0x2a, 0, 1, 0.01), testFloat, 0.01);
    TEST_EQUALS_EPS(Misb::decode (0x2a3d, 0, 1, 0.001), testFloat, 0.001);
    TEST_EQUALS_EPS(Misb::decode (0x2a3d, 0, 1, 0.0001), testFloat, 0.0001);
    TEST_EQUALS_EPS(Misb::decode (0x2a3d70, 0, 1, 0.00001), testFloat, 0.00001);
    TEST_EQUALS_EPS(Misb::decode (0x2a3d70, 0, 1, 0.000001), testFloat, 0.000001);
}

TEST_CASE(MisbExample3) {
    // Example 3 (MISB ST1201.3, section 10)
    // min = -900, max = 19000, precision = 0.5
    // expect L = 3
    // expect forwardScale = 2^8, reverseScale = 2^(-8), offzet = 0
    // expect forward mapping of 10 = 232960, or 0x038e00
    // expect reverse mapping of 0x038e00 = 10
    double min = -900, max = 19000, precision = 0.5;

    uint byteCount;
    Misb::computeByteCount (min, max, precision, &byteCount);
    TEST_EQUALS(byteCount, 3);

    double encodeScale, decodeScale, offset;
    Misb::computeScalesAndOffset (min, max, byteCount, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 8));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 8));
    TEST_EQUALS(offset, 0);

    // table 6 values for example 3
    ulong encodedValue;
    double decodedValue;

    encodedValue = Misb::encode (-900, min, max, precision);
    TEST_EQUALS(encodedValue, 0x00);
    decodedValue = Misb::decode (encodedValue, min, max, precision);
    TEST_EQUALS(decodedValue, -900.0);

    encodedValue = Misb::encode (0, min, max, precision);
    TEST_EQUALS(encodedValue, 0x038400);
    decodedValue = Misb::decode (encodedValue, min, max, precision);
    TEST_EQUALS(decodedValue, 0);

    encodedValue = Misb::encode (10, min, max, precision);
    TEST_EQUALS(encodedValue, 0x038e00);
    decodedValue = Misb::decode (encodedValue, min, max, precision);
    TEST_EQUALS(decodedValue, 10.0);

    encodedValue = Misb::encode (-numeric_limits<double>::infinity (), min, max, precision);
    TEST_EQUALS(encodedValue, 0xe80000);
    decodedValue = Misb::decode (encodedValue, min, max, precision);
    TEST_EQUALS(decodedValue, -numeric_limits<double>::infinity ());
}

TEST_CASE(MisbExample4) {
    // Example 4 (MISB ST1201.3, section 10)
    // min = 0.1, max = 0.9, L = 2
    // expect forwardScale = 2^15, reverseScale = 2^(-15), offzet = 0
    // expect forward mapping of 0.5 = 13107 or 0x3333
    // expect reverse mapping of 0x3333 = 0.499993896484375
    double min = 0.1, max = 0.9;
    uint byteCount = 2;

    double encodeScale, decodeScale, offset;
    Misb::computeScalesAndOffset (min, max, byteCount, &encodeScale, &decodeScale, &offset);
    TEST_EQUALS(encodeScale, (1 << 15));
    TEST_EQUALS(decodeScale, 1.0 / (1 << 15));
    TEST_EQUALS(offset, 0);

    // table 7 values for example 4
    ulong encodedValue;
    double decodedValue;

    encodedValue = Misb::encode (0.1, min, max, byteCount);
    TEST_EQUALS(encodedValue, 0x00);
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS_EPS(decodedValue, 0.1, 0);

    encodedValue = Misb::encode (0.5, min, max, byteCount);
    TEST_EQUALS(encodedValue, 0x3333);
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS_EPS(decodedValue, 0.5, 6.11e-6);

    encodedValue = Misb::encode (0.9, min, max, byteCount);
    TEST_EQUALS(encodedValue, 0x6666);
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS_EPS(decodedValue, 0.9, 1.221e-5);

    encodedValue = Misb::encode (-numeric_limits<double>::infinity (), min, max, byteCount);
    TEST_EQUALS(encodedValue, 0xe800);
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS(decodedValue, -numeric_limits<double>::infinity ());
}

TEST_CASE(MisbSpecialValues) {
    double min = 0.1, max = 0.9;
    uint byteCount = 3;

    double testValue;
    ulong encodedValue;
    double decodedValue;

    // this is a bit ugly, but NaN's and special values don't compare well
    // in standard C++ - we could use bit patterns, or just check that the
    // memory in the encoded value and decoded value are equivalent
    testValue = numeric_limits<double>::infinity ();
    encodedValue = Misb::encode (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_POSITIVE_INFINITY << ((byteCount - 1) * 8));
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS(*(unsigned long* )&decodedValue, *(unsigned long* )&testValue);

    testValue = -numeric_limits<double>::infinity ();
    encodedValue = Misb::encode (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_NEGATIVE_INFINITY << ((byteCount - 1) * 8));
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS(*(unsigned long* )&decodedValue, *(unsigned long* )&testValue);

    testValue = numeric_limits<double>::quiet_NaN ();
    encodedValue = Misb::encode (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_POSITIVE_QNAN << ((byteCount - 1) * 8));
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS(*(unsigned long* )&decodedValue, *(unsigned long* )&testValue);

    testValue = -numeric_limits<double>::quiet_NaN ();
    encodedValue = Misb::encode (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_NEGATIVE_QNAN << ((byteCount - 1) * 8));
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS(*(unsigned long* )&decodedValue, *(unsigned long* )&testValue);

    testValue = numeric_limits<double>::signaling_NaN ();
    encodedValue = Misb::encode (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_POSITIVE_SNAN << ((byteCount - 1) * 8));
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS(*(unsigned long* )&decodedValue, *(unsigned long* )&testValue);

    testValue = -numeric_limits<double>::signaling_NaN ();
    encodedValue = Misb::encode (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_NEGATIVE_SNAN << ((byteCount - 1) * 8));
    decodedValue = Misb::decode (encodedValue, min, max, byteCount);
    TEST_EQUALS(*(unsigned long* )&decodedValue, *(unsigned long* )&testValue);
}

TEST_CASE(MisbChecksum) {
    const byte buffer[] = { 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x02 };
    TEST_EQUALS(Misb::computeChecksum(&buffer[0], &buffer[0] + sizeof (buffer)), 0x0404);
}
