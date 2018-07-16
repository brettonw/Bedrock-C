#include "Test.h"
#include "I2C.h"

// utf-8 test file: https://www.w3.org/2001/06/utf-8-wrong/UTF-8-test.html
// https://en.wikipedia.org/wiki/UTF-8

TEST_CASE(TestDeviceI2C) {
    try {
        DeviceI2C   device (1, 0);
    }
    catch (runtime_error& exception) {
        cerr << exception.what () << endl;
        TEST_ASSERTION(true);
    }
}
