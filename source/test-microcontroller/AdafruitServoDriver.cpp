#include        "Test.h"
#include        "AdafruitServoDriver.h"
#include        "Servo.h"
#include        "TestDevice.h"
#include        "DeviceI2C.h"

template<typename DeviceType>
class TestServoDriver : public ReferenceCountedObject {
    private:
        PtrTo<AdafruitServoDriver<DeviceType> > adafruitServoDriver;
        double pulseDurations[ServoId::SERVO_15 + 1];

    public:
        TestServoDriver (PtrTo<AdafruitServoDriver<DeviceType> > _adafruitServoDriver) : adafruitServoDriver (_adafruitServoDriver) {
            for (int i = ServoId::SERVO_00; i <= ServoId::SERVO_15; ++i) {
                pulseDurations[static_cast<ServoId>(i)] = 0;
            }
        }

        void setPulseDuration (ServoId servoId, double milliseconds) {
            pulseDurations[servoId] = milliseconds;
            adafruitServoDriver->setPulseDuration (servoId, milliseconds);
        }

        double getPulseDuration (ServoId servoId) {
            return pulseDurations[servoId];
        }
};

TEST_CASE(TestAdafruitServoDriver) {
    //Log::Scope scope (Log::TRACE);
    PtrToTestDevice device = new TestDevice (ADAFRUIT_SERVO_DRIVER_DEFAULT_ADDRESS);
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
    PtrTo<TestServoDriver<TestDevice> > testDriver = new TestServoDriver<TestDevice> (driver);
    for (int i = ServoId::SERVO_00; i <= ServoId::SERVO_15; ++i) {
        TEST_XY(testDriver->getPulseDuration (static_cast<ServoId>(i)), 0);
    }

    // followed by a call to set channel pulse to set the stop position on servo 1
    device
        ->expect (0x06, (byte) 0x00)
        ->expect (0x07, (byte) 0x00)
        ->expect (0x08, (byte) 0x33)
        ->expect (0x09, (byte) 0x01);

    PtrTo<Servo<TestServoDriver<TestDevice> > > servo0 = new Servo<TestServoDriver<TestDevice> > (testDriver, ServoId::SERVO_00);
    TEST_XY(testDriver->getPulseDuration (ServoId::SERVO_00), 1.5);

    // followed by a call to set channel pulse to set the stop position
    device
        ->expect (0x0a, (byte) 0x00)
        ->expect (0x0b, (byte) 0x00)
        ->expect (0x0c, (byte) 0xcd)
        ->expect (0x0d, (byte) 0x00);

    PtrTo<Servo<TestServoDriver<TestDevice> > > servo1 = new Servo<TestServoDriver<TestDevice> > (testDriver, ServoId::SERVO_01, 0.5, 1.5);

    TEST_ASSERTION(device->report ());
}

TEST_CASE(LiveTestAdafruitServoDriver) {
    Log::Scope scope (Log::TRACE);
    try {
        PtrTo<AdafruitServoDriver<DeviceI2C> > driver = new AdafruitServoDriver<DeviceI2C> ();
        PtrTo<Servo<AdafruitServoDriver<DeviceI2C> > > servo = new Servo<AdafruitServoDriver<DeviceI2C> > (driver, ServoId::SERVO_00);
        Pause::milli (1000);
        servo->setPosition (-0.5);
        Pause::milli (1000);
        servo->setPosition (0.5);
        Pause::milli (1000);
        servo->setPosition (-0.5);
        Pause::milli (1000);
        servo->setPosition (0.5);
    } catch (...) {
    }
    TEST_X(true);
}
