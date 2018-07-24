#include "Test.h"
#include "DeviceI2C.h"
#include "AdafruitMotorDriver.h"

TEST_CASE(TestDeviceI2C) {
    try {
        Log::Scope scope (Log::TRACE);
        DeviceI2C   device (ADAFRUIT_MOTOR_DRIVER_DEFAULT_ADDRESS);

        device.write (0x43, 0x10);
        Pause::milli (10);
        byte x = device.read (0x43);
        TEST_XY(x, 0x10);
        Pause::milli (10);

        device.write (0x43, 0x11);
        Pause::milli (10);
        x = device.read (0x43);
        TEST_XY(x, 0x11);
        Pause::milli (10);

        device.write (0x43, 0x20);
        Pause::milli (10);
        x = device.read (0x43);
        TEST_XY(x, 0x20);

        Pause::milli (1000);
    }
    catch (RuntimeError& runtimeError) {
        cerr << runtimeError.getMessage () << endl;
        TEST_X(true);
    }
}
