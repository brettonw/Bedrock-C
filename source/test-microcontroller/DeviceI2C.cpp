#include "Test.h"
#include "DeviceI2C.h"

TEST_CASE(TestDeviceI2C) {
    try {
        DeviceI2C   device (1, 0);
    }
    catch (runtime_error& exception) {
        cerr << exception.what () << endl;
        TEST_ASSERTION(true);
    }
}
