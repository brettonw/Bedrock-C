#pragma once

#include        "Log.h"
#include        "RuntimeError.h"

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

        TestDevice*  begin () {
            return this;
        }

        byte read (byte at) {
            return at;
        }

        TestDevice* read (byte at, byte* out) {
            *out = read (at);
            return this;
        }

        TestDevice* write (byte at, byte b) {
            if (expectations.size () > 0) {
                Expectation& currentExpectation = expectations.front ();
                if (currentExpectation.satisfy (at, b)) {
                    Log::debug () << "TestDevice: " << "RECEIVED (" << hex (at) << ", " << hex(b) << ") -> EXPECTED" << endl;
                } else {
                    Log::error () << "TestDevice: " << "RECEIVED (" << hex (at) << ", " << hex(b) << ")" << endl;
                    throw RuntimeError (Text ("TestDevice: ") << "UNSATISFIED (" << hex (currentExpectation.address) << ", " << hex(currentExpectation.b) << ")");
                }
                expectations.erase (expectations.begin());
            } else {
                Log::error () << "TestDevice: " << "RECEIVED (" << hex (at) << ", " << hex(b) << ") -> UNEXPECTED" << endl;
                throw RuntimeError (Text ("TestDevice: ") << "UNEXPECTED");
            }

            return this;
        }

        // finish any writes
        TestDevice* flush () {
            return this;
        }

        void end () {
        }

        TestDevice* expect (byte address, byte b) {
            expectations.push_back (Expectation (address, b));
            return this;
        }

        bool report () {
            int count = expectations.size ();
            if (count > 0) {
                Expectation& currentExpectation = expectations.front ();
                Log::error () << "TestDevice: " << "UNMET (" << hex (currentExpectation.address) << ", " << hex (currentExpectation.b) << ")" << endl;
                throw RuntimeError (Text ("TestDevice: ") << "UNMET" << ((count > 1) ? "s" : "") << " (" << count << ")");
            }
            return true;
        }

};

