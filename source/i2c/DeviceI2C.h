#pragma once

#include    "Log.h"
#include    "File.h"
#include    "SMBus.h"

#define DEVICE_I2C_MAX_BUSES                256
#define DEVICE_I2C_FILE_PATH                "/dev/i2c-"

// references
// http://i2c.info/
// http://i2c.info/i2c-bus-specification
// https://www.nxp.com/docs/en/user-guide/UM10204.pdf
// https://www.kernel.org/doc/Documentation/i2c/dev-interface
// https://xanthium.in/serial-programming-tutorials
// https://www.cmrr.umn.edu/~strupp/serial.html
MAKE_PTR_TO(DeviceI2C) {
    protected:
        int smbus;

        void initDeviceI2C (Text busPath, uint address) {
            if ((smbus = SMBus::openSMBus(busPath.get ())) >= 0) {
                if (SMBus::initAddress(smbus, address) == 0) {
                    Log::info () << "DeviceI2C: " << "opened device @ (" << hex (address) << ") on " << busPath << " (smbus-" << hex (smbus) << ")" << endl;
                } else {
                    throw RuntimeError (Text ("DeviceI2C: can't open device @ (") << hex (address) << ") on " << busPath);
                }
            } else {
                throw RuntimeError (Text ("DeviceI2C: ") << "can't open smbus on " << busPath);
            }
        }

        // for testing purposes
        DeviceI2C () : smbus (-1) {}

    public:
        DeviceI2C (uint address, Text busPath) {
            initDeviceI2C (busPath, address);
        }

        DeviceI2C (uint address, uint busNumber = 0) {
            vector<Text> availableBusPaths = getAvailableBusPaths ();
            if (busNumber < availableBusPaths.size ()) {
                initDeviceI2C (availableBusPaths[busNumber], address);
            } else {
                throw RuntimeError (Text ("DeviceI2C: ") << "no I2C bus at " << hex (busNumber));
            }
        }

        static vector<Text> getAvailableBusPaths () {
            vector<Text> availableBusPaths;
            for (uint i = 0; i < DEVICE_I2C_MAX_BUSES; ++i) {
                auto smbusPath  = Text (DEVICE_I2C_FILE_PATH) << i;
                if (File (smbusPath).getExists ()) {
                    availableBusPaths.push_back (smbusPath);
                }
            }
            return availableBusPaths;
        }

        ~DeviceI2C () {
            if (smbus >= 0) {
                close (smbus);
                Log::info () << "Closed smbus (" << hex (smbus) << ")" << endl;
            }
        }

        DeviceI2C* write (byte address, byte value) {
            Log::trace () << "DeviceI2C: " << "write (@" << hex (address) << ", " << hex (value) << ")" << endl;
            SMBus::writeRegister(smbus, address, value);
            return this;
        }

        byte read (byte address) {
            int result = SMBus::readRegister(smbus, address);
            if (result >= 0) {
                Log::trace () << "DeviceI2C: " << "read (@" << hex (address) << ", got -> " << hex (result) << ")" << endl;
                return byte (0x00ff & result);
            } else {
                throw RuntimeError ("DeviceI2C: read failed");
            }
        }

};

//------------------------------------------------------------------------------------------------------
