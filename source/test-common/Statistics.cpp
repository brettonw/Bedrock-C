#include "Test.h"
#include "Statistics.h"

TEST_CASE(TestStatistics) {
    //Log::Scope scope (Log::DEBUG);

    vector<double> array;
    array.push_back(16);
    array.push_back(5);
    array.push_back(4);
    array.push_back(3);
    array.push_back(3);
    array.push_back(2);
    Statistics stats ("Test", array);
    TEST_XY(stats.getCount(), 6);
    TEST_XY(stats.getMin(), 2);
    TEST_XY(stats.getMax(), 16);
    TEST_XY(stats.getMedian(), 3.5);
    TEST_XY(stats.getMean(), 33.0 / 6.0);
    TEST_XYF(stats.getVariance(), 22.9167, 1.0e-4);
    TEST_XYF(stats.getStdDev(), sqrt (22.9167), 1.0e-5);

    Statistics stats2 ("Test", array, Statistics::ELIMINATE_OUTLIERS);
    TEST_XY(stats2.getCount(), 5);
    TEST_XY(stats2.getMin(), 2);
    TEST_XY(stats2.getMax(), 5);
    TEST_XY(stats2.getMedian(), 3);
    TEST_XY(stats2.getMean(), 17.0 / 5.0);
    TEST_XYF(stats2.getVariance(), 1.04, 1.0e-3);
    TEST_XYF(stats2.getStdDev(), sqrt (1.04), 1.0e-5);
}


