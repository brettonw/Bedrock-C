#include "Test.h"
#include "Statistics.h"

TEST_CASE(TestStatistics) {
    vector<double> array;
    array.push_back(5);
    array.push_back(4);
    array.push_back(3);
    array.push_back(3);
    array.push_back(2);
    Statistics stats ("Test", array);
    TEST_XY(stats.getCount(), 5);
    TEST_XY(stats.getMin(), 2);
    TEST_XY(stats.getMax(), 5);
    TEST_XY(stats.getMedian(), 3);
    TEST_XY(stats.getMean(), 17.0 / 5.0);
    TEST_XYF(stats.getVariance(), 1.04, DBL_EPSILON);
    TEST_XYF(stats.getStdDev(), sqrt (1.04), DBL_EPSILON );
}


