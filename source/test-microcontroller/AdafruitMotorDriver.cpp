#include        "Test.h"
#include        "AdafruitMotorDriver.h"
#include        "TestDevice.h"
#include        "DeviceI2C.h"
#include        "Motor.h"

template<typename DeviceType>
class TestMotorDriver : public ReferenceCountedObject {
    private:
        PtrTo<AdafruitMotorDriver<DeviceType> > adafruitMotorDriver;
        double speeds[MotorId::MOTOR_3 + 1];

    public:
        TestMotorDriver (PtrTo<AdafruitMotorDriver<DeviceType> > _adafruitMotorDriver) : adafruitMotorDriver (_adafruitMotorDriver) {
            for (int i = MotorId::MOTOR_0; i <= MotorId::MOTOR_3; ++i) {
                speeds[static_cast<MotorId>(i)] = 0;
            }
        }

        void runMotor (MotorId motorId, double speed) {
            speeds[motorId] = speed;
            adafruitMotorDriver->runMotor (motorId, speed);
        }

        double getSpeed (MotorId motorId) {
            return speeds[motorId];
        }
};

TEST_CASE(TestAdafruitMotorDriver) {
    Log::Scope scope (Log::TRACE);

    PtrToTestDevice device = new TestDevice (ADAFRUIT_MOTOR_DRIVER_DEFAULT_ADDRESS);
    device
        // constructor
        ->expect (0xfa, 0x00)
        ->expect (0xfb, 0x00)
        ->expect (0xfc, 0x00)
        ->expect (0xfd, 0x00)
        ->expect (0x01, 0x04)
        ->expect (0x00, 0x01)
        ->expect (0x00, 0x00)
        // which calls setPulseFrequency
        ->expect (0x00, (byte) 0x10)
        ->expect (0xfe, (byte) 0x05)
        ->expect (0x00, (byte) 0x00)
        ->expect (0x00, (byte) 0x80)
        // followed by a set of calls to stop all the motors
        ->expect (0x2a, (byte) 0x00)
        ->expect (0x2b, (byte) 0x00)
        ->expect (0x2c, (byte) 0x00)
        ->expect (0x2d, (byte) 0x10)
        ->expect (0x2e, (byte) 0x00)
        ->expect (0x2f, (byte) 0x00)
        ->expect (0x30, (byte) 0x00)
        ->expect (0x31, (byte) 0x10)
        ->expect (0x26, (byte) 0x00)
        ->expect (0x27, (byte) 0x00)
        ->expect (0x28, (byte) 0x00)
        ->expect (0x29, (byte) 0x10)

        ->expect (0x36, (byte) 0x00)
        ->expect (0x37, (byte) 0x00)
        ->expect (0x38, (byte) 0x00)
        ->expect (0x39, (byte) 0x10)
        ->expect (0x32, (byte) 0x00)
        ->expect (0x33, (byte) 0x00)
        ->expect (0x34, (byte) 0x00)
        ->expect (0x35, (byte) 0x10)
        ->expect (0x3a, (byte) 0x00)
        ->expect (0x3b, (byte) 0x00)
        ->expect (0x3c, (byte) 0x00)
        ->expect (0x3d, (byte) 0x10)

        ->expect (0x12, (byte) 0x00)
        ->expect (0x13, (byte) 0x00)
        ->expect (0x14, (byte) 0x00)
        ->expect (0x15, (byte) 0x10)
        ->expect (0x16, (byte) 0x00)
        ->expect (0x17, (byte) 0x00)
        ->expect (0x18, (byte) 0x00)
        ->expect (0x19, (byte) 0x10)
        ->expect (0x0e, (byte) 0x00)
        ->expect (0x0f, (byte) 0x00)
        ->expect (0x10, (byte) 0x00)
        ->expect (0x11, (byte) 0x10)

        ->expect (0x1e, (byte) 0x00)
        ->expect (0x1f, (byte) 0x00)
        ->expect (0x20, (byte) 0x00)
        ->expect (0x21, (byte) 0x10)
        ->expect (0x1a, (byte) 0x00)
        ->expect (0x1b, (byte) 0x00)
        ->expect (0x1c, (byte) 0x00)
        ->expect (0x1d, (byte) 0x10)
        ->expect (0x22, (byte) 0x00)
        ->expect (0x23, (byte) 0x00)
        ->expect (0x24, (byte) 0x00)
        ->expect (0x25, (byte) 0x10);

    PtrTo<AdafruitMotorDriver<TestDevice> > driver = new AdafruitMotorDriver<TestDevice> (device);
    PtrTo<TestMotorDriver<TestDevice> > testDriver = new TestMotorDriver<TestDevice> (driver);
    for (int i = MotorId::MOTOR_0; i <= MotorId::MOTOR_3; ++i) {
        TEST_XY(testDriver->getSpeed (static_cast<MotorId>(i)), 0);
    }

    // stop motor 0
    device
        ->expect (0x2a, (byte) 0x00)
        ->expect (0x2b, (byte) 0x00)
        ->expect (0x2c, (byte) 0x00)
        ->expect (0x2d, (byte) 0x10)
        ->expect (0x2e, (byte) 0x00)
        ->expect (0x2f, (byte) 0x00)
        ->expect (0x30, (byte) 0x00)
        ->expect (0x31, (byte) 0x10)
        ->expect (0x26, (byte) 0x00)
        ->expect (0x27, (byte) 0x00)
        ->expect (0x28, (byte) 0x00)
        ->expect (0x29, (byte) 0x10);

    PtrTo<Motor<TestMotorDriver<TestDevice> > > motor = new Motor<TestMotorDriver<TestDevice> > (testDriver, MotorId::MOTOR_0);
    TEST_XY(testDriver->getSpeed (MotorId::MOTOR_0), 0);

    // (runMotor) - motorId: MOTOR_1, speed: 1.0
    device
       ->expect (0x2a, (byte) 0x00)
       ->expect (0x2b, (byte) 0x10)
       ->expect (0x2c, (byte) 0x00)
       ->expect (0x2d, (byte) 0x00)
       ->expect (0x2e, (byte) 0x00)
       ->expect (0x2f, (byte) 0x00)
       ->expect (0x30, (byte) 0x00)
       ->expect (0x31, (byte) 0x10)
       ->expect (0x26, (byte) 0x00)
       ->expect (0x27, (byte) 0x10)
       ->expect (0x28, (byte) 0x00)
       ->expect (0x29, (byte) 0x00);
     motor->run (1.0);
    TEST_XY(testDriver->getSpeed (MotorId::MOTOR_0), 1.0);

    // (runMotor) - motorId: MOTOR_1, speed: 1.5
    device
       ->expect (0x2a, (byte) 0x00)
       ->expect (0x2b, (byte) 0x10)
       ->expect (0x2c, (byte) 0x00)
       ->expect (0x2d, (byte) 0x00)
       ->expect (0x2e, (byte) 0x00)
       ->expect (0x2f, (byte) 0x00)
       ->expect (0x30, (byte) 0x00)
       ->expect (0x31, (byte) 0x10)
       ->expect (0x26, (byte) 0x00)
       ->expect (0x27, (byte) 0x10)
       ->expect (0x28, (byte) 0x00)
       ->expect (0x29, (byte) 0x00);
     motor->run (1.5);
    TEST_XY(testDriver->getSpeed (MotorId::MOTOR_0), 1.0);

    // the PCA9685 class otherwise exposes no useful public interface to test
    TEST_ASSERTION(device->report ());
}

TEST_CASE(LiveTestAdafruitMotorDriver) {
    try {
    } catch (...) {
    }
    TEST_X(true);
}
