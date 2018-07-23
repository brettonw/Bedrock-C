#include "Test.h"
#include "Pause.h"

TEST_CASE(TestPauses) {
    auto startTime = chrono::high_resolution_clock::now();
    Pause::micro (500);
    auto stopTime = chrono::high_resolution_clock::now();
    int delta = static_cast<int> (chrono::duration_cast<chrono::microseconds>(stopTime - startTime).count ());
    TEST_XYF(delta, 500, 10);

}
