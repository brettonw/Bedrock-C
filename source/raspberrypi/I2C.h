#include    "Types.h"

#include <unistd.h>

#ifndef __APPLE__
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#endif

#include <sys/ioctl.h>
#include <fcntl.h>

#pragma once

#define DEVICE_I2C_ERROR                    -1
#define DEVICE_I2C_BUFFER_SIZE              256
#define DEVICE_I2C_FILE_PATH_BUFFER_SIZE    32
#define DEVICE_I2C_FILE_PATH                "/dev/i2c-%d"

// XXX TODO: is it possible to open the device file descriptor and keep it open?
class DeviceI2C {
    private:
        byte buffer[DEVICE_I2C_BUFFER_SIZE];
        uint length;
        int device;

        void store (byte b) {
            if ((length + 1) <= DEVICE_I2C_BUFFER_SIZE) {
                buffer[length] = b;
            } else {
                throw runtime_error ("DeviceI2C: out of buffer for writes");
            }
        }

    public:
        // deviceNumber may need to always be 1
        DeviceI2C (uint deviceNumber, uint deviceAddress) : length (0) {
            // open the device, configure it to use 7 bit addresses (per i2c-dev.h), and set the
            // slave address to our requested I2C address
            char devicePath[DEVICE_I2C_FILE_PATH_BUFFER_SIZE];
            snprintf(devicePath, DEVICE_I2C_FILE_PATH_BUFFER_SIZE, DEVICE_I2C_FILE_PATH, deviceNumber);
            device = open (devicePath, O_RDWR);
            if (((device = open (devicePath, O_RDWR)) >= 0) && (ioctl(device, I2C_TENBIT, 0) >= 0) && (ioctl(device, I2C_SLAVE, deviceAddress) >= 0)) {
                cerr << "Opened device " << devicePath << " at address 0x" << hex << deviceAddress << " (0x" << hex << device << ")" << endl;
            } else {
                throw runtime_error ("DeviceI2C: can't open device");
            }
        }

        ~DeviceI2C () {
            if (device >= 0) {
                close (device);
                cerr << "Closed device (0x" << hex << device << ")" << endl;
            }
        }

        DeviceI2C& write (byte address, byte value) {
            store (address);
            store (value);
            return *this;
        }

        bool flush () {
            if (::write (device, buffer, length) == length) {
                length = 0;
                return true;
            }
            return false;
        }

        byte read (byte address) {
            // append the write command to set the target address to the write buffer, this will
            // have the nice side effect of finishing any pending writes.
            store (address);
            if ((flush ()) && (::read (device, &buffer[0], 1) == 1)) {
                return buffer[0];
            }
            throw runtime_error ("DeviceI2C: read failed");
        }

};
