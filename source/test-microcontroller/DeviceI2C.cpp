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
