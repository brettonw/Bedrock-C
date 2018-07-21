#pragma once

#include        "Log.h"
#include        "ServoId.h"

template<typename DeviceType>
class Servo : public ReferenceCountedObject {
    private:
        ServoId servoId;
        PtrTo<DeviceType> device;
        double minPositionMs;
        double maxPositionMs;
        double position;

    public:

    /**
     * contructor to use the default signal min and max values of 1ms and 2ms which are typical of
     * all servos, as near as I can tell. However, there is a lot of variance from servo to servo,
     * so this is really just a starting point for most servos.
     * @param servoController the controller to use for this servo
     * @param servoId         the id corresponding to the driver pins for this servo on the controller
     */
    Servo (PtrTo<DeviceType> _device, ServoId _servoId, double _minPositionMs = 1.0, double _maxPositionMs = 2.0) : device (_device), servoId (_servoId), minPositionMs (_minPositionMs), maxPositionMs (_maxPositionMs) {
        setPosition (0);
    }

    /**
     * @param position - in the range -1..1
     * @return
     */
    Servo setPosition (double position) {
        this.position = position = Utility.clamp (position, -1, 1);
        double pulseDurationMilliseconds = min + ((max - min) * (position + 1) * 0.5);
        log.trace (servoId.name () + "@" + String.format ("%.04f", pulseDurationMilliseconds));
        servoController.setPulseDuration (servoId, pulseDurationMilliseconds);
        return this;
    }

    /**
     * @return
     */
    double getPosition () {
        return position;
    }

    /**
     * @return
     */
    ServoId getServoId () {
        return servoId;
    }

};
