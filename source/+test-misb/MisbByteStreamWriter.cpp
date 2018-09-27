#include "Test.h"
#include "MisbByteStreamReader.h"
#include "MisbByteStreamWriter.h"

TEST_MODULE_DEPENDENCIES (MisbByteStreamWriter, "MisbByteStream,WriteSupplier")

#define MISB_BYTE_STREAM_WRITER_X MisbByteStreamWriter0
#define MISB_BYTE_STREAM_READER_X MisbByteStreamReader0

TEST_CASE(MisbByteStreamWriteGeneric) {
    // make a test stream... it's a bit longer than we need so we
    // can make a few assertions and tests without *actually*
    // stomping memory on the stack
    byte buffer[256];

    // some test data
    const char* testString1 = "Hello World! ";
    const char* testString2 = "No Sugar! ";
    const char* testString3 = "No Calories! ";
    uint testString1Length = strlen (testString1);
    uint testString2Length = strlen (testString2);
    uint testString3Length = strlen (testString3);
    uint totalLength = testString1Length + testString2Length + testString3Length;

    // fill the stream with some test data
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, 128);
    mbsw.writeBytes (testString1Length, testString1);
    mbsw.writeBytes (testString2Length, testString2);
    mbsw.writeBytes (testString3Length, testString3);
    TEST_EQUALS(mbsw.finish (), totalLength);
    TEST_EQUALS(memcmp (buffer, "Hello World! No Sugar! No Calories! ", totalLength), 0);
}

TEST_CASE(MisbByteStreamWrite) {
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
    uint compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);
    Log::debug () << "Check all individual values..." << endl;
    for (uint i = 0; i < compareLength; ++i) {
        TEST_EQUALS((int ) buffer[i], (int ) compare[i]);
    }
}

TEST_CASE(MisbByteStreamWrite2) {
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
    uint compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);
    Log::debug () << "Check all individual values..." << endl;
    for (uint i = 0; i < compareLength; ++i) {
        TEST_EQUALS((int ) buffer[i], (int ) compare[i]);
    }
}

TEST_CASE(MisbByteStreamWriteBerA) {
    byte buffer[256];
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, sizeof(buffer) / sizeof(buffer[0]));
    ulong bytesWritten = mbsw.writeUV (200).finish ();

    byte compare[] = { 0x01, 0xc8 };
    uint compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);
}

TEST_CASE(MisbByteStreamWriteBerB) {
    byte buffer[256];
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, sizeof(buffer) / sizeof(buffer[0]));
    ulong bytesWritten = mbsw.writeUV (123456).finish ();

    byte compare[] = { 0x03, 0x01, 0xe2, 0x40 };
    uint compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);
}

TEST_CASE(MisbByteStreamWriteBerOID) {
    // (MISB ST0601.12, section 6.5.2.3, figures 6-4, 6-5, and 6-6)
    byte buffer[256];
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, sizeof(buffer) / sizeof(buffer[0]));
    mbsw.writeOid (98);
    mbsw.writeOid (144);
    mbsw.writeOid (23298);
    ulong bytesWritten = mbsw.finish ();

    byte compare[] = { 0x62, 0x81, 0x10, 0x81, 0xb6, 0x02 };
    uint compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);

}

TEST_CASE(MisbByteStreamWriteBerOID130) {
    // (MISB ST0601.12, section 6.5.2.3, figures 6-4, 6-5, and 6-6)
    byte buffer[256];
    MISB_BYTE_STREAM_WRITER_X mbsw (buffer, sizeof(buffer) / sizeof(buffer[0]));
    mbsw.writeOid (130);
    ulong bytesWritten = mbsw.finish ();

    byte compare[] = { 0x81, 0x02 };
    uint compareLength = sizeof(compare) / sizeof(compare[0]);
    TEST_EQUALS(bytesWritten, compareLength);
    TEST_EQUALS(memcmp (&buffer[0], &compare[0], compareLength), 0);
}

TEST_CASE(MisbByteStreamWriteChecksum) {
    TEST_NYI;
}
