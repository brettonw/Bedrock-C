#pragma once

// headers needed for open, close, and ioctl
#include    <fcntl.h>
#include    <unistd.h>
#include    <sys/ioctl.h>

#ifndef __APPLE__
#include    <linux/i2c.h>
#include    <linux/i2c-dev.h>
#else
// shamelessly copied from a linux i2c.h and i2c-dev.h headers to facilitate compilation and unit
// testing on platforms other than linux, or that don't have an I2C bus. prefer the linux source if
// it is available

#define     I2C_SLAVE               0x0703  // use this slave address
#define     I2C_TENBIT              0x0704  // set to 0 for 7 bit addrs (pretty much everything we care about)
#define     I2C_SMBUS               0x0720  // perform a SMBus transfer

// SMBus read or write markers
#define     I2C_SMBUS_READ          1
#define     I2C_SMBUS_WRITE         0

// sizes of transfers for byte and byte data
#define     I2C_SMBUS_BYTE          1
#define     I2C_SMBUS_BYTE_DATA     2

#endif

class SMBus {
    private:
        struct SMBusControl {
            byte readWrite;
            byte command;
            uint size;
            byte* data;

            SMBusControl (byte _readWrite, byte _command, uint _size, byte*_data) :
                readWrite (_readWrite), command (_command), size (_size), data(_data) {}
        };

        static int read (int smbus, byte command, int size, byte* data) {
            SMBusControl control (I2C_SMBUS_READ, command, size, data);
            return ioctl (smbus, I2C_SMBUS, &control);
        }

        static int write (int smbus, byte command, int size, byte* data) {
            SMBusControl control (I2C_SMBUS_WRITE, command, size, data);
            return ioctl (smbus, I2C_SMBUS, &control);
        }

    public:
        static int openSMBus (const char* path) {
            return open (path, O_RDWR);
        }

        static int closeSMBus (int smbus) {
            return close (smbus);
        }

        static int initAddress (int smbus, uint address) {
            return ((ioctl (smbus, I2C_TENBIT, 0) == 0) && (ioctl (smbus, I2C_SLAVE, address) == 0)) ? 0 : -1;
        }

        static int readByte (int smbus) {
            byte data;
            return (read (smbus, 0, I2C_SMBUS_BYTE, &data) == 0) ? (0x00ff & data) : -1;
        }

        static int writeByte (int smbus, byte value) {
            return write (smbus, value, I2C_SMBUS_BYTE, 0);
        }

        static int readRegister (int smbus, byte deviceRegister) {
            byte data[2];
            return (read (smbus, deviceRegister, I2C_SMBUS_BYTE_DATA, &data[0]) == 0) ? (0x00ff & data[0]) : -1;
        }

        static int writeRegister (int smbus, byte deviceRegister, byte value) {
            byte data[2] = {value, 0x00};
            return write (smbus, deviceRegister, I2C_SMBUS_BYTE_DATA, &data[0]);
        }
};
