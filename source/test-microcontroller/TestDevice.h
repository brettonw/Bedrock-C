#pragma once

#include        "Text.h"

class Expectation {
    public:
        byte address;
        byte b;

        Expectation (byte _address, byte _b) : address (_address), b (_b) {
        }

        bool satisfy (int _address, byte _b) {
            return (_address == address) && (_b == b);
        }
};

MAKE_PTR_TO(TestDevice) {
    private:
        vector<Expectation> expectations;

    public:
        TestDevice (uint address, Text busPath) {
        }

        TestDevice (uint address, uint busNumber = 0) {
        }

        ~TestDevice () {
        }

        TestDevice* write (byte address, byte b) {
            cerr << "TestDevice: " << "RECEIVED (" << hex (address) << ", " << hex(b) << ")";
            if (expectations.size () > 0) {
                Expectation& currentExpectation = expectations.front ();
                if (currentExpectation.satisfy (address, b)) {
                    //log.debug ("EXPECTED! (" + String.format ("0x%04x", address) + ", " + String.format ("0x%02x", b) + ")");
                    cerr << " -> EXPECTED" << endl;
                } else {
                    throw runtime_error (Text ("TestDevice: ") << "UNSATISFIED (" << hex (currentExpectation.address) << ", " << hex(currentExpectation.b) << ")");
                }
                expectations.erase (expectations.begin());
            } else {
                throw runtime_error (Text ("TestDevice: ") << "UNEXPECTED");
            }

            return this;
        }

        bool flush () {
            return true;
        }

        byte read (byte address) {
            return address;
        }

        TestDevice* expect (byte address, byte b) {
            expectations.push_back (Expectation (address, b));
            return this;
        }

        void report () {
            int count = expectations.size ();
            if (count > 0) {
                Expectation& currentExpectation = expectations.front ();
                cerr << "TestDevice: " << "UNMET (" << hex (currentExpectation.address) << ", " << hex (currentExpectation.b) << ")" << endl;
                throw runtime_error (Text ("TestDevice: ") << "UNMET" << ((count > 1) ? "s" : "") << " (" << count << ")");
            }
        }
};

