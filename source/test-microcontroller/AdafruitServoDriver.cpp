#include        "Test.h"
#include        "AdafruitServoDriver.h"
#include        "Servo.h"
#include        "TestDevice.h"

TEST_CASE(TestAdafruitServoDriver) {
    //Log::Scope scope (Log::TRACE);
    PtrToTestDevice device = new TestDevice (ADAFRUIT_SERVER_DRIVER_DEFAULT_ADDRESS);
    device
        // PCA9685 constructor
        ->expect (0xfa, 0x00)
        ->expect (0xfb, 0x00)
        ->expect (0xfc, 0x00)
        ->expect (0xfd, 0x00)
        ->expect (0x01, 0x04)
        ->expect (0x00, 0x01)
        ->expect (0x00, 0x00)
        // which calls setPulseFrequency
        ->expect (0x00, (byte) 0x10)
        ->expect (0xfe, (byte) 0x79)
        ->expect (0x00, (byte) 0x00)
        ->expect (0x00, (byte) 0x80);

    PtrTo<AdafruitServoDriver<TestDevice> > driver = new AdafruitServoDriver<TestDevice> (device);

    // followed by a call to set channel pulse to set the stop position
    device
        ->expect (0x06, (byte) 0x00)
        ->expect (0x07, (byte) 0x00)
        ->expect (0x08, (byte) 0x33)
        ->expect (0x09, (byte) 0x01);

    PtrTo<Servo<AdafruitServoDriver<TestDevice> > > servo = new Servo<AdafruitServoDriver<TestDevice> > (driver, ServoId::SERVO_01);

    // the PCA9685 class otherwise exposes no useful public interface to test
    TEST_ASSERTION(device->report ());
}
