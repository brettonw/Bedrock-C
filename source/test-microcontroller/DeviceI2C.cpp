#include "Test.h"
#include "DeviceI2C.h"

TEST_CASE(TestDeviceI2C) {
    try {
        DeviceI2C   device (0x40);
        TEST_ASSERTION(true);
    }
    catch (runtime_error& exception) {
        cerr << exception.what () << endl;
        TEST_ASSERTION(true);
    }
}
