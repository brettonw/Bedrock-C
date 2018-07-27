#include        "Test.h"
#include        "AdafruitServoDriver.h"
#include        "Servo.h"
#include        "NullDevice.h"
#include        "DeviceI2C.h"

TEST_CASE(TestServo) {
    //Log::Scope scope (Log::TRACE);
    PtrToNullDevice device = new NullDevice ();

    PtrTo<AdafruitServoDriver<NullDevice> > driver = new AdafruitServoDriver<NullDevice> (device);
    for (int i = ServoId::SERVO_00; i <= ServoId::SERVO_15; ++i) {
        TEST_XY(driver->getPulseDuration (static_cast<ServoId>(i)), 0);
    }

    PtrTo<Servo<AdafruitServoDriver<NullDevice> > > servo0 = new Servo<AdafruitServoDriver<NullDevice> > (driver, ServoId::SERVO_00);
    TEST_XY(servo0->getServoId (), ServoId::SERVO_00);
    TEST_XY(servo0->getPosition (), 0);
    TEST_XY(driver->getPulseDuration (ServoId::SERVO_00), 1.5);

    PtrTo<Servo<AdafruitServoDriver<NullDevice> > > servo1 = new Servo<AdafruitServoDriver<NullDevice> > (driver, ServoId::SERVO_01, 0.5, 1.5);
    TEST_XY(servo1->getServoId (), ServoId::SERVO_01);
    TEST_XY(servo1->getPosition (), 0);
    TEST_XY(driver->getPulseDuration (ServoId::SERVO_01), 1.0);
}

TEST_CASE(LiveTestServo) {
    //Log::Scope scope (Log::TRACE);
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
    } catch (RuntimeError& runtimeError) {
        Log::exception (runtimeError);
    } catch (...) {
    }
    TEST_X(true);
}
