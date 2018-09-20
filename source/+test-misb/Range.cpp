#include "Test.h"
#include "Range.h"

TEST_CASE(Range) {
    Range range (1, 2);
    TEST_EQUALS(range.getMin (), 1);
    TEST_EQUALS(range.getMax (), 2);
    TEST_EQUALS(Range::DEFAULT.getMin (), 0);
    TEST_EQUALS(Range::DEFAULT.getMax (), 1);
}

