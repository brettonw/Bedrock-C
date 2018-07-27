#include "Test.h"
#include "Pause.h"

void testMicro (int microseconds) {
    int sum = 0;
    int sampleCount = 100;
    for (int i = 0; i < sampleCount; ++i) {
        auto startTime = chrono::high_resolution_clock::now();
        Pause::micro (microseconds);
        auto stopTime = chrono::high_resolution_clock::now();
        int delta = static_cast<int> (chrono::duration_cast<chrono::microseconds>(stopTime - startTime).count ());
        sum += delta;
    }
    Log::debug () << "testMicro: " << "requested " << microseconds << " us pause, waited for " << (sum / sampleCount) << " us on average over " << sampleCount << " pauses" << endl;
    // ensure it is within some minimal degree of error (2 / sampleCount)
    TEST_XYF(sum, (microseconds * sampleCount), (2 * microseconds));
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

void testMilli (int milliseconds) {
    int sum = 0;
    int sampleCount = 20;
    for (int i = 0; i < sampleCount; ++i) {
        auto startTime = chrono::high_resolution_clock::now();
        Pause::milli (milliseconds);
        auto stopTime = chrono::high_resolution_clock::now();
        int delta = static_cast<int> (chrono::duration_cast<chrono::milliseconds>(stopTime - startTime).count ());
        sum += delta;
    }
    Log::debug () << "testMilli: " << "requested " << milliseconds << " ms pause, waited for " << (sum / sampleCount) << " ms on average over " << sampleCount << " pauses" << endl;
    // ensure it is within some minimal degree of error (3 / sampleCount)
    TEST_XYF(sum, (milliseconds * sampleCount), (3 * milliseconds));
}

TEST_CASE(TestMilliPause) {
    Log::Scope  scope (Log::DEBUG);
    testMilli (10);
    testMilli (20);
    testMilli (50);
    testMilli (100);
    testMilli (200);
}


