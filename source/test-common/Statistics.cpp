#include "Test.h"
#include "Statistics.h"

TEST_CASE(TestStatistics) {
    Log::Scope scope (Log::DEBUG);

    vector<double> array;
    array.push_back(16);
    array.push_back(5);
    array.push_back(4);
    array.push_back(3);
    array.push_back(3);
    array.push_back(2);

    {
        Statistics stats ("Test", array);
        TEST_XY(stats.getCount(), 6);
        TEST_XY(stats.getMin(), 2);
        TEST_XY(stats.getMax(), 16);
        TEST_XYU(stats.getMedian(), (3.0 + 4.0) / 2.0);
        double mean = 33.0 / 6.0;
        TEST_XYU(stats.getMean(), mean);
        double variance = (((16 - mean) * (16 - mean)) + ((5 - mean) * (5 - mean)) + ((4 - mean) * (4 - mean)) + (2 * ((3 - mean) * (3 - mean))) + ((2 - mean) * (2 - mean))) / 6.0;
        TEST_XYU(stats.getVariance(), variance);
        TEST_XYU(stats.getStdDev(), sqrt (variance));
    }

    {
        Statistics stats ("Test", array, Statistics::ELIMINATE_OUTLIERS);
        TEST_XY(stats.getCount(), 5);
        TEST_XY(stats.getMin(), 2);
        TEST_XY(stats.getMax(), 5);
        TEST_XY(stats.getMedian(), 3);
        double mean = 17.0 / 5.0;
        TEST_XYU(stats.getMean(), mean);
        double variance = (((5 - mean) * (5 - mean)) + ((4 - mean) * (4 - mean)) + (2 * ((3 - mean) * (3 - mean))) + ((2 - mean) * (2 - mean))) / 5.0;
        TEST_XYU(stats.getVariance(), variance);
        TEST_XYU(stats.getStdDev(), sqrt (variance));
    }
}


