#pragma once

#include        "PCA9685.h"

/**
* Servo Driver Board
*
* https://learn.adafruit.com/16-channel-pwm-servo-driver/overview
*
* this breakout board is a straightforward implementation of a 9685 16-Channel Pulse Width
* Modulation Controller (PWM) for LEDs with 12-bits of resolution. We use it to provide a bunch of
* PWM outputs for servos.
*
*/

enum ServoId {
    SERVO_01, SERVO_02, SERVO_03, SERVO_04,
    SERVO_05, SERVO_06, SERVO_07, SERVO_08,
    SERVO_09, SERVO_10, SERVO_11, SERVO_12,
    SERVO_13, SERVO_14, SERVO_15, SERVO_16
};

template<typename DeviceType>
public class AdafruitServoDriver : public PCA9685<DeviceType> {
    private:
        // default pulse frequency of the Adafruit server hat
        enum {
            DEFAULT_PULSE_FREQUENCY = 50
        }


    public:
        // default i2c address of the Adafruit server hat
        enum {
            DEFAULT_ADDRESS = 0x40,
            DEFAULT_PULSE_FREQUENCY = 50
        }

        AdafruitServoDriver (uint address = DEFAULT_ADDRESS, uint requestedPulseFrequency = DEFAULT_PULSE_FREQUENCY, uint busNumber = 0) : PCA9685 (address, requestedPulseFrequency, busNumber) {}

        AdafruitServoDriver (PtrTo<DeviceType> _device, uint requestedPulseFrequency = DEFAULT_PULSE_FREQUENCY) : PCA9685 (_device, requestedPulseFrequency) {}

        /**
        * set the pulse width to control a servo. the exact meaning of this is up to the servo itself.
        * @param servoId - which servo to set the pulse duration for
        * @param milliseconds - the width of the puls in milliseconds
        * @return this, for chaining
        */
        void setPulseDuration (ServoId servoId, double milliseconds) {
            setChannelPulseMs (servoId, milliseconds);
        }
}

typedef PtrTo<AdafruitServoDriver<DeviceI2C> > PtrToAdafruitServoDriver;
