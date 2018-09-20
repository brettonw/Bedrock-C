#include "Test.h"
#include "MisbByteStreamReader.h"
#include "MisbByteStreamWriter.h"

#define MISB_BYTE_STREAM_WRITER_X MisbByteStreamWriter0
#define MISB_BYTE_STREAM_READER_X MisbByteStreamReader0

TEST_CASE(MisbByteStreamReadGeneric) {
    // make a test stream... it's a bit longer than we need so we
    // can make a few assertions and tests without *actually*
    // stomping memory on the stack
    byte buffer[256];

    // some test data
    const char* testString1 = "Hello World! ";
    const char* testString2 = "No Sugar! ";
    const char* testString3 = "No Calories! ";
    int testString1Length = strlen (testString1);
    int testString2Length = strlen (testString2);
    int testString3Length = strlen (testString3);
    int totalLength = testString1Length + testString2Length + testString3Length;

    // fill the stream with some test data
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, 128);
    mbsw.writeBytes (testString1Length, testString1);
    mbsw.writeBytes (testString2Length, testString2);
    mbsw.writeBytes (testString3Length, testString3);
    TEST_EQUALS(mbsw.finish (), totalLength);
    TEST_EQUALS(memcmp (buffer, "Hello World! No Sugar! No Calories! ", totalLength), 0);

    // read some test data back from the stream
    byte readBuffer[256];
    MISB_BYTE_STREAM_READER_X mbsr (buffer, mbsw.size ());
    mbsr.readBytes (testString1Length, readBuffer);
    TEST_EQUALS(memcmp (readBuffer, testString1, testString1Length), 0);
    mbsr.readBytes (testString2Length, readBuffer);
    TEST_EQUALS(memcmp (readBuffer, testString2, testString2Length), 0);
    mbsr.readBytes (testString3Length, readBuffer);
    TEST_EQUALS(memcmp (readBuffer, testString3, testString3Length), 0);
    TEST_EQUALS(mbsr.finish (), totalLength);
}

TEST_CASE(MisbByteStreamRead) {
    // make a test stream... it's a bit longer than we need so we
    // can make a few assertions and tests without *actually*
    // stomping memory on the stack
    byte buffer[256];

    // fill the stream with some integer values
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, 128);
    mbsw
            .writeUF1 (0x00ef)
            .writeUF2 (0x00beef)
            .writeUF3 (0x00adbeef)
            .writeUF4 (0x00deadbeefL)
            .writeUF5 (0x00efdeadbeefL)
            .writeUF6 (0x00beefdeadbeefL)
            .writeUF7 (0x00adbeefdeadbeefL)
            .writeUF8 (0xdeadbeefdeadbeefL);
    // 36 bytes so far
    uint bytesWritten = mbsw.finish ();
    TEST_EQUALS(bytesWritten, 36);

    // check that the memory contains what we expect
    byte compare[] = { 0xef, 0xbe, 0xef, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef };
    int compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);
    Log::debug () << "Check all individual values..." << endl;
    for (int i = 0; i < compareLength; ++i) {
        TEST_EQUALS((int ) buffer[i], (int ) compare[i]);
    }

    // check that the read back returns the original values
    ulong readV;
    MISB_BYTE_STREAM_READER_X mbsr (buffer, 128);
    mbsr.readUF1 (&readV);
    TEST_EQUALS(readV, 0x00ef);
    mbsr.readUF2 (&readV);
    TEST_EQUALS(readV, 0x00beef);
    mbsr.readUF3 (&readV);
    TEST_EQUALS(readV, 0x00adbeef);
    mbsr.readUF4 (&readV);
    TEST_EQUALS(readV, 0x00deadbeefL);
    mbsr.readUF5 (&readV);
    TEST_EQUALS(readV, 0x00efdeadbeefL);
    mbsr.readUF6 (&readV);
    TEST_EQUALS(readV, 0x00beefdeadbeefL);
    mbsr.readUF7 (&readV);
    TEST_EQUALS(readV, 0x00adbeefdeadbeefL);
    mbsr.readUF8 (&readV);
    TEST_EQUALS(readV, 0xdeadbeefdeadbeefL);
    TEST_EQUALS(mbsr.finish (), 36);

    // write some floats (10 bytes)
    double testFloat = 1.0 + (1.0 / 3.0);
    TEST_EQUALS(mbsw.begin ().setRange (1, 2).writeFF1 (testFloat).writeFF2 (testFloat).writeFF3 (testFloat).writeFF4 (testFloat).finish (), 10);

    double rF1, rF2, rF3, rF4;
    TEST_EQUALS(mbsr.begin ().setRange (1, 2).readFF1 (&rF1).readFF2 (&rF2).readFF3 (&rF3).readFF4 (&rF4).finish (), 10);
    TEST_EQUALS_EPS(rF1, testFloat, 1.0 / (1 << 7));
    TEST_EQUALS_EPS(rF2, testFloat, 1.0 / (1 << 15));
    TEST_EQUALS_EPS(rF3, testFloat, 1.0 / (1 << 23));
    TEST_EQUALS_EPS(rF4, testFloat, 1.0 / (1L << 31));
}

TEST_CASE(MisbByteStreamRead2) {
    // fill the stream with some integer values
    MisbByteStreamWriter2 mbsw;
    mbsw
            .writeUF1 (0x00ef)
            .writeUF2 (0x00beef)
            .writeUF3 (0x00adbeef)
            .writeUF4 (0x00deadbeefL)
            .writeUF5 (0x00efdeadbeefL)
            .writeUF6 (0x00beefdeadbeefL)
            .writeUF7 (0x00adbeefdeadbeefL)
            .writeUF8 (0xdeadbeefdeadbeefL);
    // 36 bytes so far
    uint bytesWritten = mbsw.finish ();
    TEST_EQUALS(bytesWritten, 36);
    byte* buffer = mbsw.getOrigin();

    // check that the memory contains what we expect
    byte compare[] = { 0xef, 0xbe, 0xef, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef };
    int compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);
    Log::debug () << "Check all individual values..." << endl;
    for (int i = 0; i < compareLength; ++i) {
        TEST_EQUALS((int ) buffer[i], (int ) compare[i]);
    }

    // check that the read back returns the original values
    ulong readV;
    MISB_BYTE_STREAM_READER_X mbsr (buffer, 128);
    mbsr.readUF1 (&readV);
    TEST_EQUALS(readV, 0x00ef);
    mbsr.readUF2 (&readV);
    TEST_EQUALS(readV, 0x00beef);
    mbsr.readUF3 (&readV);
    TEST_EQUALS(readV, 0x00adbeef);
    mbsr.readUF4 (&readV);
    TEST_EQUALS(readV, 0x00deadbeefL);
    mbsr.readUF5 (&readV);
    TEST_EQUALS(readV, 0x00efdeadbeefL);
    mbsr.readUF6 (&readV);
    TEST_EQUALS(readV, 0x00beefdeadbeefL);
    mbsr.readUF7 (&readV);
    TEST_EQUALS(readV, 0x00adbeefdeadbeefL);
    mbsr.readUF8 (&readV);
    TEST_EQUALS(readV, 0xdeadbeefdeadbeefL);
    TEST_EQUALS(mbsr.finish (), 36);
}

TEST_CASE(MisbByteStreamReadX) {
    // make a test stream... it's a bit longer than we need so we
    // can make a few assertions and tests without *actually*
    // stomping memory on the stack
    byte buffer[256];

    // fill the stream with some integer values
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, 128);
    mbsw
            .writeUF (1, 0x00ef)
            .writeUF (2, 0x00beef)
            .writeUF (3, 0x00adbeef)
            .writeUF (4, 0x00deadbeefL)
            .writeUF (5, 0x00efdeadbeefL)
            .writeUF (6, 0x00beefdeadbeefL)
            .writeUF (7, 0x00adbeefdeadbeefL)
            .writeUF (8, 0xdeadbeefdeadbeefL);
    // 36 bytes so far
    uint bytesWritten = mbsw.finish ();
    TEST_EQUALS(bytesWritten, 36);

    // check that the memory contains what we expect
    byte compare[] = { 0xef, 0xbe, 0xef, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef };
    int compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);
    Log::debug () << "Check all individual values..." << endl;
    for (int i = 0; i < compareLength; ++i) {
        TEST_EQUALS((int ) buffer[i], (int ) compare[i]);
    }

    // check that the read back returns the original values
    ulong readV;
    MISB_BYTE_STREAM_READER_X mbsr (buffer, 128);
    mbsr.readUF (1, &readV);
    TEST_EQUALS(readV, 0x00ef);
    mbsr.readUF (2, &readV);
    TEST_EQUALS(readV, 0x00beef);
    mbsr.readUF (3, &readV);
    TEST_EQUALS(readV, 0x00adbeef);
    mbsr.readUF (4, &readV);
    TEST_EQUALS(readV, 0x00deadbeefL);
    mbsr.readUF (5, &readV);
    TEST_EQUALS(readV, 0x00efdeadbeefL);
    mbsr.readUF (6, &readV);
    TEST_EQUALS(readV, 0x00beefdeadbeefL);
    mbsr.readUF (7, &readV);
    TEST_EQUALS(readV, 0x00adbeefdeadbeefL);
    mbsr.readUF (8, &readV);
    TEST_EQUALS(readV, 0xdeadbeefdeadbeefL);
    TEST_EQUALS(mbsr.finish (), 36);

    // write some floats (10 bytes)
    double testFloat = 1.0 + (1.0 / 3.0);
    TEST_EQUALS(mbsw.begin ().setRange (1, 2).writeFF (1, testFloat).writeFF (2, testFloat).writeFF (3, testFloat).writeFF (4, testFloat).finish (), 10);

    double rF1, rF2, rF3, rF4;
    TEST_EQUALS(mbsr.begin ().setRange (1, 2).readFF (1, &rF1).readFF (2, &rF2).readFF (3, &rF3).readFF (4, &rF4).finish (), 10);
    TEST_EQUALS_EPS(rF1, testFloat, 1.0 / (1 << 7));
    TEST_EQUALS_EPS(rF2, testFloat, 1.0 / (1 << 15));
    TEST_EQUALS_EPS(rF3, testFloat, 1.0 / (1 << 23));
    TEST_EQUALS_EPS(rF4, testFloat, 1.0 / (1L << 31));
}

TEST_CASE(MisbByteStreamGet) {
    // make a test stream... it's a bit longer than we need so we
    // can make a few assertions and tests without *actually*
    // stomping memory on the stack
    byte buffer[256];

    // fill the stream with some integer values
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, 128);
    mbsw
            .writeUF (1, 0x00ef)
            .writeUF (2, 0x00beef)
            .writeUF (3, 0x00adbeef)
            .writeUF (4, 0x00deadbeefL)
            .writeUF (5, 0x00efdeadbeefL)
            .writeUF (6, 0x00beefdeadbeefL)
            .writeUF (7, 0x00adbeefdeadbeefL)
            .writeUF (8, 0xdeadbeefdeadbeefL);
    // 36 bytes so far
    uint bytesWritten = mbsw.finish ();
    TEST_EQUALS(bytesWritten, 36);

    // check that the memory contains what we expect
    byte compare[] = { 0xef, 0xbe, 0xef, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef };
    int compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);
    Log::debug () << "Check all individual values..." << endl;;
    for (int i = 0; i < compareLength; ++i) {
        TEST_EQUALS((int ) buffer[i], (int ) compare[i]);
    }

    // check that the gets return the original values
    MISB_BYTE_STREAM_READER_X mbsr (buffer, 128);
    ulong value;
    value = mbsr.getUF (1);
    TEST_EQUALS(value, 0x00ef);
    value = mbsr.getUF (2);
    TEST_EQUALS(value, 0x00beef);
    value = mbsr.getUF (3);
    TEST_EQUALS(value, 0x00adbeef);
    value = mbsr.getUF (4);
    TEST_EQUALS(value, 0x00deadbeefL);
    value = mbsr.getUF (5);
    TEST_EQUALS(value, 0x00efdeadbeefL);
    value = mbsr.getUF (6);
    TEST_EQUALS(value, 0x00beefdeadbeefL);
    value = mbsr.getUF (7);
    TEST_EQUALS(value, 0x00adbeefdeadbeefL);
    value = mbsr.getUF (8);
    TEST_EQUALS(value, 0xdeadbeefdeadbeefL);
    TEST_EQUALS(mbsr.finish (), 36);

    // write some floats (10 bytes)
    double testFloat = 1.0 + (1.0 / 3.0);
    TEST_EQUALS(mbsw.begin ().setRange (1, 2).writeFF (1, testFloat).writeFF (2, testFloat).writeFF (3, testFloat).writeFF (4, testFloat).finish (), 10);

    // get some floats
    mbsr.begin ().setRange (1, 2);
    double valueD;
    valueD = mbsr.getFF (1);
    TEST_EQUALS_EPS(valueD, testFloat, 1.0 / (1 << 7));
    valueD = mbsr.getFF (2);
    TEST_EQUALS_EPS(valueD, testFloat, 1.0 / (1 << 15));
    valueD = mbsr.getFF (3);
    TEST_EQUALS_EPS(valueD, testFloat, 1.0 / (1 << 23));
    valueD = mbsr.getFF (4);
    TEST_EQUALS_EPS(valueD, testFloat, 1.0 / (1L << 31));
    TEST_EQUALS(mbsr.finish (), 10);
}

TEST_CASE(MisbByteStreamReadBerA) {
    byte buffer[256];
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, sizeof(buffer) / sizeof(buffer[0]));
    ulong bytesWritten = mbsw.writeUV (200).finish ();

    byte compare[] = { 0x01, 0xc8 };
    int compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);

    MISB_BYTE_STREAM_READER_X mbsr (buffer, sizeof(buffer) / sizeof(buffer[0]));
    ulong result = mbsr.getUV ();
    TEST_EQUALS(result, 200);
}

TEST_CASE(MisbByteStreamReadBerB) {
    byte buffer[256];
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, sizeof(buffer) / sizeof(buffer[0]));
    ulong bytesWritten = mbsw.writeUV (123456).finish ();

    byte compare[] = { 0x03, 0x01, 0xe2, 0x40 };
    int compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);

    MISB_BYTE_STREAM_READER_X mbsr (buffer, sizeof(buffer) / sizeof(buffer[0]));
    ulong result = mbsr.getUV ();
    TEST_EQUALS(result, 123456);
}

TEST_CASE(MisbByteStreamReadBerOID) {
    // (MISB ST0601.12, section 6.5.2.3, figures 6-4, 6-5, and 6-6)
    byte buffer[256];
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, sizeof(buffer) / sizeof(buffer[0]));
    mbsw.writeOid (98);
    mbsw.writeOid (144);
    mbsw.writeOid (23298);
    ulong bytesWritten = mbsw.finish ();

    byte compare[] = { 0x62, 0x81, 0x10, 0x81, 0xb6, 0x02 };
    int compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);

    MISB_BYTE_STREAM_READER_X mbsr (buffer, sizeof(buffer) / sizeof(buffer[0]));
    ulong value;
    value = mbsr.getOid ();
    TEST_EQUALS(value, 98);
    value = mbsr.getOid ();
    TEST_EQUALS(value, 144);
    value = mbsr.getOid ();
    TEST_EQUALS(value, 23298);
}

TEST_CASE(MisbByteStreamReadChecksum) {
}

