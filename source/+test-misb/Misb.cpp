#include "Test.h"
#include "Misb.h"

TEST_MODULE_DEPENDENCIES (Misb, "Range")

TEST_CASE(Misb1201ComputeByteBount) {
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

TEST_CASE(Misb1201ComputeScalesAndOffset) {
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

TEST_CASE(Misb1201EncodeDecode) {

    // test the obvious in the range 0..1
    // test in the range -1..1
    // test in the range 1000..2000
    double testFloat = 0.33;
    TEST_EQUALS(Misb::encode1201 (testFloat, 0, 1, 0.1), 0x2a);
    TEST_EQUALS(Misb::encode1201 (testFloat, 0, 1, 0.01), 0x2a);
    TEST_EQUALS(Misb::encode1201 (testFloat, 0, 1, 0.001), 0x2a3d);
    TEST_EQUALS(Misb::encode1201 (testFloat, 0, 1, 0.0001), 0x2a3d);
    TEST_EQUALS(Misb::encode1201 (testFloat, 0, 1, 0.00001), 0x2a3d70);
    TEST_EQUALS(Misb::encode1201 (testFloat, 0, 1, 0.000001), 0x2a3d70);

    TEST_EQUALS_EPS(Misb::decode1201 (0x2a, 0, 1, 0.1), testFloat, 0.1);
    TEST_EQUALS_EPS(Misb::decode1201 (0x2a, 0, 1, 0.01), testFloat, 0.01);
    TEST_EQUALS_EPS(Misb::decode1201 (0x2a3d, 0, 1, 0.001), testFloat, 0.001);
    TEST_EQUALS_EPS(Misb::decode1201 (0x2a3d, 0, 1, 0.0001), testFloat, 0.0001);
    TEST_EQUALS_EPS(Misb::decode1201 (0x2a3d70, 0, 1, 0.00001), testFloat, 0.00001);
    TEST_EQUALS_EPS(Misb::decode1201 (0x2a3d70, 0, 1, 0.000001), testFloat, 0.000001);
}

TEST_CASE(Misb1201Example3) {
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

    encodedValue = Misb::encode1201 (-900, min, max, precision);
    TEST_EQUALS(encodedValue, 0x00);
    decodedValue = Misb::decode1201 (encodedValue, min, max, precision);
    TEST_EQUALS(decodedValue, -900.0);

    encodedValue = Misb::encode1201 (0, min, max, precision);
    TEST_EQUALS(encodedValue, 0x038400);
    decodedValue = Misb::decode1201 (encodedValue, min, max, precision);
    TEST_EQUALS(decodedValue, 0);

    encodedValue = Misb::encode1201 (10, min, max, precision);
    TEST_EQUALS(encodedValue, 0x038e00);
    decodedValue = Misb::decode1201 (encodedValue, min, max, precision);
    TEST_EQUALS(decodedValue, 10.0);

    encodedValue = Misb::encode1201 (-numeric_limits<double>::infinity (), min, max, precision);
    TEST_EQUALS(encodedValue, 0xe80000);
    decodedValue = Misb::decode1201 (encodedValue, min, max, precision);
    TEST_EQUALS(decodedValue, -numeric_limits<double>::infinity ());
}

TEST_CASE(Misb1201Example4) {
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

    encodedValue = Misb::encode1201 (0.1, min, max, byteCount);
    TEST_EQUALS(encodedValue, 0x00);
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS_EPS(decodedValue, 0.1, 0);

    encodedValue = Misb::encode1201 (0.5, min, max, byteCount);
    TEST_EQUALS(encodedValue, 0x3333);
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS_EPS(decodedValue, 0.5, 6.11e-6);

    encodedValue = Misb::encode1201 (0.9, min, max, byteCount);
    TEST_EQUALS(encodedValue, 0x6666);
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS_EPS(decodedValue, 0.9, 1.221e-5);

    encodedValue = Misb::encode1201 (-numeric_limits<double>::infinity (), min, max, byteCount);
    TEST_EQUALS(encodedValue, 0xe800);
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS(decodedValue, -numeric_limits<double>::infinity ());
}

TEST_CASE(Misb1201SpecialValues) {
    double min = 0.1, max = 0.9;
    uint byteCount = 3;

    double testValue;
    ulong encodedValue;
    double decodedValue;

    // this is a bit ugly, but NaN's and special values don't compare well
    // in standard C++ - we could use bit patterns, or just check that the
    // memory in the encoded value and decoded value are equivalent
    testValue = numeric_limits<double>::infinity ();
    encodedValue = Misb::encode1201 (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_POSITIVE_INFINITY << ((byteCount - 1) * 8));
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS(union_cast<ulong> (decodedValue), union_cast<ulong> (testValue));

    testValue = -numeric_limits<double>::infinity ();
    encodedValue = Misb::encode1201 (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_NEGATIVE_INFINITY << ((byteCount - 1) * 8));
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS(union_cast<ulong> (decodedValue), union_cast<ulong> (testValue));

    testValue = numeric_limits<double>::quiet_NaN ();
    encodedValue = Misb::encode1201 (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_POSITIVE_QNAN << ((byteCount - 1) * 8));
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS(union_cast<ulong> (decodedValue), union_cast<ulong> (testValue));

    testValue = -numeric_limits<double>::quiet_NaN ();
    encodedValue = Misb::encode1201 (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_NEGATIVE_QNAN << ((byteCount - 1) * 8));
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS(union_cast<ulong> (decodedValue), union_cast<ulong> (testValue));

    testValue = numeric_limits<double>::signaling_NaN ();
    encodedValue = Misb::encode1201 (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_POSITIVE_SNAN << ((byteCount - 1) * 8));
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS(union_cast<ulong> (decodedValue), union_cast<ulong> (testValue));

    testValue = -numeric_limits<double>::signaling_NaN ();
    encodedValue = Misb::encode1201 (testValue, min, max, byteCount);
    TEST_EQUALS(encodedValue, Misb::SPECIAL_NEGATIVE_SNAN << ((byteCount - 1) * 8));
    decodedValue = Misb::decode1201 (encodedValue, min, max, byteCount);
    TEST_EQUALS(union_cast<ulong> (decodedValue), union_cast<ulong> (testValue));
}

TEST_CASE(MisbChecksum) {
    const byte buffer[] = { 0x01, 0x01, 0x01, 0x02, 0x00, 0x01, 0x02 };
    TEST_EQUALS(Misb::computeChecksum(&buffer[0], &buffer[0] + sizeof (buffer)), 0x0404);
}

void decodeBytes0601 (initializer_list<byte> bytes, uint byteCount, double min, double max, double expect, double epsilon = 1.0e-4) {
    const byte* valueBytes = bytes.begin();
    ulong encodedValue = 0;
    for (uint i = 0; i < byteCount; ++i) {
        encodedValue = (encodedValue << 8) | valueBytes[i];
    }
    double decodedValue = Misb::decode0601(encodedValue, min, max, byteCount);
    TEST_EQUALS_EPS (decodedValue, expect, epsilon);
}

void decodeBytes1201 (initializer_list<byte> bytes, uint byteCount, double min, double max, double expect, double epsilon = 1.0e-4) {
    const byte* valueBytes = bytes.begin();
    ulong encodedValue = 0;
    for (uint i = 0; i < byteCount; ++i) {
        encodedValue = (encodedValue << 8) | valueBytes[i];
    }
    double decodedValue = Misb::decode1201(encodedValue, min, max, byteCount);
    TEST_EQUALS_EPS (decodedValue, expect, epsilon);
}

TEST_CASE(From0601) {
    // tag 5 (360 / 65535)
    decodeBytes0601 ({0x71, 0xc2}, 2, 0, 360, 159.974365);
    // tag 6 (40 / 65534)
    decodeBytes0601 ({0xfd, 0x3d}, 2, -20, 20, -0.43153);
    // tag 7 (100 / 65534)
    decodeBytes0601 ({0x08, 0xb8}, 2, -50, 50, 3.40587);
    // tag 13 (180 / 4294967294)
    decodeBytes0601 ({0x55, 0x95, 0xb6, 0x6d}, 4, -90, 90, 60.17682);
    // tag 14 (360 / 4294967294)
    decodeBytes0601 ({0x5b, 0x53, 0x60, 0xc4}, 4, -180, 180, 128.42676);
    // tag 15 (19900 / 65535)
    decodeBytes0601 ({0xc2, 0x21}, 2, -900, 19000, 14190.7195);
    // tag 16 (180 / 65535)
    decodeBytes0601 ({0xcd, 0x9c}, 2, 0, 180, 144.57130);
    // tag 17 (180 / 65535)
    decodeBytes0601 ({0xd9, 0x17}, 2, 0, 180, 152.64363);
    // tag 18 (360 / 429...294)
    decodeBytes0601 ({0x72, 0x4a, 0x0a, 0x20}, 4, 0, 360, 160.719211);
    // tag 19 (360 / 4294967294)
    decodeBytes0601 ({ 0x87, 0xf8, 0x4b, 0x86}, 4, -180, 180, -168.79232);
    // tag 20 (360 / 4294967295)
    decodeBytes0601 ({0x7d, 0xc5, 0x5e, 0xce}, 4, 0, 360, 176.86544);
    // tag 21 (5000000 / 4294967295)
    decodeBytes0601 ({0x03, 0x83, 0x09, 0x26}, 4, 0, 5000000, 68590.98330);
    // tag 22 (10000 / 65535)
    decodeBytes0601 ({0x12, 0x81}, 2, 0, 10000, 722.81987);
    // tag 23 (180 / 4294967294)
    decodeBytes0601 ({0xf1, 0x01, 0xa2, 0x29}, 4, -90, 90, -10.542389);
    // tag 24 (360 / 4294967294)
    decodeBytes0601 ({0x14, 0xbc, 0x08, 0x2b}, 4, -180, 180, 29.15789);
    // tag 25 (19900 / 65535)
    decodeBytes0601 ({0x34, 0xf3}, 2, -900, 19000, 3216.03723);
    // tag 26 (0.15 / 65534) - uses tag 23 value in the calculation
    decodeBytes0601 ({0xc0, 0x6e}, 2, -0.075, 0.075, -10.5796380 - -10.542389);
    // tag 27 (0.15 / 65534) - uses tag 24 value in the calculation
    decodeBytes0601 ({0xcb, 0xe9}, 2, -0.075, 0.075, 29.12737 - 29.15789);
    // tag 28 (0.15 / 65534) - uses tag 23 value in the calculation
    decodeBytes0601 ({0xd7, 0x65}, 2, -0.075, 0.075, -10.56618 - -10.542389);
    // tag 29 (0.15 / 65534) - uses tag 24 value in the calculation
    decodeBytes0601 ({0xe2, 0xe0}, 2, -0.075, 0.075, 29.14082 - 29.15789);
    // tag 30 (0.15 / 65534) - uses tag 23 value in the calculation
    decodeBytes0601 ({0xee, 0x5b}, 2, -0.075, 0.075, -10.55273 - -10.542389);
    // tag 31 (0.15 / 65534) - uses tag 24 value in the calculation
    decodeBytes0601 ({0xf9, 0xd6}, 2, -0.075, 0.075, 29.15428 - 29.15789);
    // tag 32 (0.15 / 65534) - uses tag 23 value in the calculation
    decodeBytes0601 ({0x05, 0x52}, 2, -0.075, 0.075, -10.53927 - -10.542389);
    // tag 33 (0.15 / 65534) - uses tag 24 value in the calculation
    decodeBytes0601 ({0x10, 0xcd}, 2, -0.075, 0.075, 29.16773 - 29.15789);
    // tag 35 (360 / 65535)
    decodeBytes0601 ({0xa7, 0xc4}, 2, 0, 360, 235.92401);
    // tag 36 (100 / 255)
    decodeBytes0601 ({0xb2}, 1, 0, 100, 69.8039216);
    // tag 37 (5000 / 65535)
    decodeBytes0601 ({0xbe, 0xba}, 2, 0, 5000, 3725.18502);
    // tag 38 (19900 / 65535)
    decodeBytes0601 ({0xca, 0x35}, 2, -900, 19000, 14818.6770);
    // tag 40 (180 / 4294967294)
    decodeBytes0601 ({0x8f, 0x69, 0x52, 0x62}, 4, -90, 90, -79.16385);
    // tag 41 (360 / 4294967294)
    decodeBytes0601 ({0x76, 0x54, 0x57, 0x92}, 4, -180, 180, 166.40081);
    // tag 42 (19900 / 65535)
    decodeBytes0601 ({0xf8, 0x23}, 2, -900, 19000, 18389.0471);
    // tag 45 (4095 / 65535)
    decodeBytes0601 ({0x1a, 0x95}, 2, 0, 4095, 425.21515);
    // tag 46 (4095 / 65535)
    decodeBytes0601 ({0x26, 0x11}, 2, 0, 4095, 608.9231);
    // tag 49 (5000 / 65535)
    decodeBytes0601 ({0x3d, 0x07}, 2, 0, 5000, 1191.95850);
    // tag 50 (40 / 65534)
    decodeBytes0601 ({0xc8, 0x83}, 2, -20, 20, -8.67031);
    // tag 51 (360 / 65534)
    decodeBytes0601 ({0xd3, 0xfe}, 2, -180, 180, -61.88788);
    // tag 52 (40 / 65534)
    decodeBytes0601 ({0xdf, 0x79}, 2, -20, 20, -5.08255);
    // tag 53 (5000 / 65535)
    decodeBytes0601 ({0x6a, 0xf4}, 2, 0, 5000, 2088.96010);
    // tag 54 (19900 / 65535)
    decodeBytes0601 ({0x76, 0x70}, 2, -900, 19000, 8306.80552);
    // tag 55 (19900 / 65535)
    decodeBytes0601 ({0x81}, 1, 0, 100, 50.58824);
    // tag 57 (5000000 / 4294967295)
    decodeBytes0601 ({0xb3, 0x8e, 0xac, 0xf1}, 4, 0, 5000000, 3506979.03161);
    // tag 58 (10000 / 65535)
    decodeBytes0601 ({0xa4, 0x5d}, 2, 0, 10000, 6420.53864);
    // tag 64 (360 / 65535)
    decodeBytes0601 ({0xdd, 0xc5}, 2, 0, 360, 311.86816);
    // tag 67 (180 / 4294967294)
    decodeBytes0601 ({0x85, 0xa1, 0x5a, 0x39}, 4, -90, 90, -86.04121);
    // tag 68 (360 / 4294967294)
    decodeBytes0601 ({0x00, 0x1c, 0x50, 0x1c}, 4, -180, 180, 0.15553);
    // tag 69 (19900 / 65535)
    decodeBytes0601 ({0x0b, 0xb3}, 2, -900, 19000, 9.44533);
    // tag 71 (360 / 65535)
    decodeBytes0601 ({0x17, 0x2f}, 2, 0, 360, 32.60243);
    // tag 75 (19900 / 65535)
    decodeBytes0601 ({0xc2, 0x21}, 2, -900, 19000, 14190.7195);
    // tag 76 (19900 / 65535)
    decodeBytes0601 ({0x0b, 0xb3}, 2, -900, 19000, 9.44533);
    // tag 78 (19900 / 65535)
    decodeBytes0601 ({0x0b, 0xb3}, 2, -900, 19000, 9.44533);
    // tag 79 (654 / 65534)
    decodeBytes0601 ({0x09, 0xfb}, 2, -327, 327, 25.49776);
    // tag 82 (180 / 4294967294)
    decodeBytes0601 ({0xf0, 0xf4, 0x12, 0x44}, 4, -90, 90, -10.57964);
    // tag 83 (360 / 4294967294)
    decodeBytes0601 ({0x14, 0xb6, 0x79, 0xb9}, 4, -180, 180, 29.12737);
    // tag 84 (180 / 4294967294)
    decodeBytes0601 ({0xf0, 0xf8, 0xf8, 0x7e}, 4, -90, 90, -10.56618);
    // tag 85 (360 / 4294967294)
    decodeBytes0601 ({0x14, 0xb8, 0xec, 0xd6}, 4, -180, 180, 29.14082);
    // tag 86 (180 / 4294967294)
    decodeBytes0601 ({0xf0, 0xfd, 0xde, 0x81}, 4, -90, 90, -10.55273);
    // tag 87 (360 / 4294967294)
    decodeBytes0601 ({0x14, 0xbb, 0x5f, 0xd8}, 4, -180, 180, 29.15428);
    // tag 88 (180 / 4294967294)
    decodeBytes0601 ({0xf1, 0x02, 0xc4, 0xbb}, 4, -90, 90, -10.53927);
    // tag 89 (360 / 4294967294)
    decodeBytes0601 ({0x14, 0xbd, 0xd2, 0xf5}, 4, -180, 180, 29.16773);
    // tag 90 (180 / 4294967294)
    decodeBytes0601 ({0xff, 0x62, 0xe2, 0xf2}, 4, -90, 90, -0.43153);
    // tag 91 (180 / 4294967294)
    decodeBytes0601 ({0x04, 0xd8, 0x04, 0xdf}, 4, -90, 90, 3.40581);
    // tag 92 (180 / 4294967294)
    decodeBytes0601 ({0xf3, 0xab, 0x48, 0xef}, 4, -90, 90, -8.67018);

    // tag 96
    decodeBytes1201 ({0x00, 0xd9, 0x2a}, 3, 0, 1500000, 13898.5);
    // tag 103, 104, 105
    decodeBytes1201 ({0x2f, 0x92, 0x1e}, 3, -900, 40000, 23456.24, 1.0e-2);
    // tag 109
    decodeBytes1201 ({0x00, 0x01, 0xa0}, 3, 0, 21000, 1.625, 1.0e-2);
    // tag 112
    decodeBytes1201 ({0x1f, 0x40}, 2, 0, 360, 125, 1.0e-1);
    // tag 113
    decodeBytes1201 ({0x05, 0xf5, 0x00}, 3, -900, 40000, 2150, 1.0e-1);
    // tag 114
    decodeBytes1201 ({0x05, 0xf7, 0x40}, 3, -900, 40000, 2154.5, 1.0e-1);
    // tag 117
    decodeBytes1201 ({0x3e, 0x90}, 2, -1000, 1000, 1, 1.0e-1);
    // tag 118
    decodeBytes1201 ({0x3e, 0x80, 0x11}, 3, -1000, 1000, 0.00418);
    // tag 119
    decodeBytes1201 ({0x3b, 0x60}, 2, -1000, 1000, -50, 1.0e-1);
    // tag 120
    decodeBytes1201 ({0x48, 0x00}, 2, 0, 100, 72, 1.0e-1);
    // tag 132
    decodeBytes1201 ({0x02, 0x57, 0xc0}, 3, 1, 99999, 2400, 1.0e-1);
    // tag 134
    decodeBytes1201 ({0x37, 0x00}, 2, 0, 100, 55, 1.0e-1);
}


