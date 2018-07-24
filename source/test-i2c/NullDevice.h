#pragma once

#include        "Text.h"

MAKE_PTR_TO(NullDevice) {
    public:
        NullDevice () {}

        NullDevice (uint address, uint busNumber = 0) {}

        ~NullDevice () {}

        NullDevice* write (byte address, byte b) {
            return this;
        }

        byte read (byte address) {
            return address;
        }
};

