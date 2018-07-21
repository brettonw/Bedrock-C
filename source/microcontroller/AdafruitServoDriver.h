#pragma once

#include        "PCA9685.h"
#include        "ServoId.h"

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

const int ADAFRUIT_SERVER_DRIVER_DEFAULT_ADDRESS = 0x40;
const int ADAFRUIT_SERVER_DRIVER_DEFAULT_PULSE_FREQUENCY = 50;

template<typename DeviceType>
class AdafruitServoDriver : public PCA9685<DeviceType> {
    public:
        AdafruitServoDriver (uint address = ADAFRUIT_SERVER_DRIVER_DEFAULT_ADDRESS, uint requestedPulseFrequency = ADAFRUIT_SERVER_DRIVER_DEFAULT_PULSE_FREQUENCY, uint busNumber = 0) : PCA9685<DeviceType> (address, requestedPulseFrequency, busNumber) {}

        AdafruitServoDriver (PtrTo<DeviceType> _device, uint requestedPulseFrequency = ADAFRUIT_SERVER_DRIVER_DEFAULT_PULSE_FREQUENCY) : PCA9685<DeviceType> (_device, requestedPulseFrequency) {}

        /**
        * set the pulse width to control a servo. the exact meaning of this is up to the servo itself.
        * @param servoId - which servo to set the pulse duration for
        * @param milliseconds - the width of the puls in milliseconds
        * @return this, for chaining
        */
        void setPulseDuration (ServoId servoId, double milliseconds) {
            PCA9685<DeviceType>::setChannelPulseMs (servoId, milliseconds);
        }
};
