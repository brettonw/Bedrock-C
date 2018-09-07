#include "Test.h"
#include "Statistics.h"

TEST_CASE(TestStatistics) {
    //Log::Scope scope (Log::DEBUG);

    vector<f4> array4;
    array4.push_back(16);
    array4.push_back(5);
    array4.push_back(4);
    array4.push_back(3);
    array4.push_back(3);
    array4.push_back(2);

    vector<f8> array8;
    array8.push_back(16);
    array8.push_back(5);
    array8.push_back(4);
    array8.push_back(3);
    array8.push_back(3);
    array8.push_back(2);

    {
        Log::debug() << "F8 - Keep Outliers" << endl;
        Statistics<f8> stats ("Test", array8);
        TEST_XY(stats.getCount(), 6);
        TEST_XY(stats.getMin(), 2);
        TEST_XY(stats.getMax(), 16);
        TEST_XYULP(stats.getMedian(), (3.0 + 4.0) / 2.0);
        f8 mean = 33.0 / 6.0;
        TEST_XYULP(stats.getMean(), mean);
        f8 variance = (((16 - mean) * (16 - mean)) + ((5 - mean) * (5 - mean)) + ((4 - mean) * (4 - mean)) + (2 * ((3 - mean) * (3 - mean))) + ((2 - mean) * (2 - mean))) / 6.0;
        TEST_XYULP(stats.getVariance(), variance);
        TEST_XYULP(stats.getStdDev(), sqrt (variance));
    }

    {
        Log::debug() << "F4 - Keep Outliers" << endl;
        Statistics<f4> stats ("Test", array4);
        TEST_XY(stats.getCount(), 6);
        TEST_XY(stats.getMin(), 2);
        TEST_XY(stats.getMax(), 16);
        TEST_XYULP(stats.getMedian(), f4 ((3.0 + 4.0) / 2.0));
        f4 mean = 33.0 / 6.0;
        TEST_XYULP(stats.getMean(), mean);
        f4 variance = (((16 - mean) * (16 - mean)) + ((5 - mean) * (5 - mean)) + ((4 - mean) * (4 - mean)) + (2 * ((3 - mean) * (3 - mean))) + ((2 - mean) * (2 - mean))) / 6.0;
        TEST_XYULP(stats.getVariance(), variance);
        TEST_XYULP(stats.getStdDev(), sqrt (variance));
    }

    {
        Log::debug() << "F8 - Remove Outliers" << endl;
        Statistics<f8> stats ("Test", array8, Statistics<f8>::ELIMINATE_OUTLIERS);
        TEST_XY(stats.getCount(), 5);
        TEST_XY(stats.getMin(), 2);
        TEST_XY(stats.getMax(), 5);
        TEST_XY(stats.getMedian(), 3);
        f8 mean = 17.0 / 5.0;
        TEST_XYULP(stats.getMean(), mean);
        f8 variance = (((5 - mean) * (5 - mean)) + ((4 - mean) * (4 - mean)) + (2 * ((3 - mean) * (3 - mean))) + ((2 - mean) * (2 - mean))) / 5.0;
        TEST_XYULP(stats.getVariance(), variance);
        TEST_XYULP(stats.getStdDev(), sqrt (variance));
    }

    {
        Log::debug() << "F4 - Remove Outliers" << endl;
        Statistics<f4> stats ("Test", array4, Statistics<f4>::ELIMINATE_OUTLIERS);
        TEST_XY(stats.getCount(), 5);
        TEST_XY(stats.getMin(), 2);
        TEST_XY(stats.getMax(), 5);
        TEST_XY(stats.getMedian(), 3);
        f4 mean = 17.0 / 5.0;
        TEST_XYULP(stats.getMean(), mean);
        f4 variance = (((5 - mean) * (5 - mean)) + ((4 - mean) * (4 - mean)) + (2 * ((3 - mean) * (3 - mean))) + ((2 - mean) * (2 - mean))) / 5.0;
        TEST_XYULP(stats.getVariance(), variance);
        TEST_XYULP(stats.getStdDev(), sqrt (variance));
    }
}


