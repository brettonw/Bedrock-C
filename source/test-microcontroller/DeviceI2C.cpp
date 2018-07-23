#include "Test.h"
#include "DeviceI2C.h"

TEST_CASE(TestDeviceI2C) {
    try {
        DeviceI2C   device (0x40);
        TEST_ASSERTION(true);
    }
    catch (RuntimeError& runtimeError) {
        cerr << runtimeError.getMessage () << endl;
        TEST_X(true);
    }
}

class TestDeviceI2C : public DeviceI2C {
    public:
        TestDeviceI2C () {}

        bool flush () {
            length = 0;
            return true;
        }

        bool expect (byte* source, uint sourceLength) {
            if (sourceLength == length) {
                return (memcmp (buffer, source, length) == 0);
            } else {
                {
                    Log::info () << "Source (" << sourceLength;
                    for (uint i = 0; i < sourceLength; ++i) {
                        Log::debug () << ", " << hex(source[i]);
                    }
                    Log::info () << ")" << endl;
                }

                {
                    Log::info () << "Buffer (" << length;
                    for (uint i = 0; i < length; ++i) {
                        Log::debug () << ", " << hex(buffer[i]);
                    }
                    Log::info () << ")" << endl;
                }
            }
            return false;
        }
};


TEST_CASE(TestDeviceI2CBuffer) {
    TestDeviceI2C   device;

    device.write (0x01, 0x02);
    byte testBuf[] = { 0x01, 0x02 };
    TEST_X (device.expect(testBuf, 2));

    device.write (0x02, 0x03);
    byte testBuf2[] = { 0x01, 0x02, 0x02, 0x03 };
    TEST_X (device.expect(testBuf2, 4));

    device.flush ();
    device.write (0x01, 0x02);
    TEST_X (device.expect(testBuf, 2));
}
