#include "Test.h"
#include "PCA9685.h"

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

TEST_CASE(TestPCA9685) {
    cerr << "XXX" << endl;
    try {
        PtrToTestDevice device = new TestDevice (0x40);
        device
            // constructor
            ->expect (0xfa, 0x00)
            ->expect (0xfb, 0x00)
            ->expect (0xfc, 0x00)
            ->expect (0xfd, 0x00)
            ->expect (0x01, 0x04)
            ->expect (0x00, 0x01)
            ->expect (0x00, 0x00)
            // setPulseFrequency
            ->expect (0x0000, (byte) 0x10)
            ->expect (0x00fe, (byte) 0x05)
            ->expect (0x0000, (byte) 0x00)
            ->expect (0x0000, (byte) 0x80);

        PtrTo<PCA9685<TestDevice> > pca9685 = new PCA9685<TestDevice> (device);
        device->report ();
        TEST_ASSERTION(true);
    }
    catch (runtime_error& exception) {
        cerr << exception.what () << endl;
    }
}
