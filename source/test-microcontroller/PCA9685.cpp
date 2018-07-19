#include "Test.h"
#include "TestDevice.h"
#include "PCA9685.h"

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
