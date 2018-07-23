#include "Test.h"
#include "Pause.h"

void testMicro (int microseconds) {
    int average = 0;
    int sampleCount = 10;
    for (int i = 0; i < sampleCount; ++i) {
        auto startTime = chrono::high_resolution_clock::now();
        Pause::micro (microseconds);
        auto stopTime = chrono::high_resolution_clock::now();
        int delta = static_cast<int> (chrono::duration_cast<chrono::microseconds>(stopTime - startTime).count ());
        average += delta;
    }
    average /= sampleCount;
    Log::debug () << "testMicro: " << "requested " << microseconds << " us pause, waited for " << average << " us on average over " << sampleCount << " pauses" << endl;
    // ensure it is within 5% error
    TEST_XYF(average, microseconds, microseconds / 20);
}

TEST_CASE(TestMicroPause) {
    Log::Scope  scope (Log::DEBUG);
    testMicro (10);
    testMicro (20);
    testMicro (50);
    testMicro (100);
    testMicro (200);
    testMicro (300);
    testMicro (500);
}
