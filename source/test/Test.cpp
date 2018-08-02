#include "Test.h"

/*
TEST_CASE(ForceFailure) {
    TEST_ASSERTION(false);
}
*/

UnitTest* UnitTest::currentUnitTest = 0;

TEST_CASE(TestTypeSizes) {
    // these tests should pass if compilation succeeded - there are static asserts around their
    // declaration that cover the numbers
    TEST_XY(sizeof (u1), 1);
    TEST_XY(sizeof (u2), 2);
    TEST_XY(sizeof (u4), 4);
    TEST_XY(sizeof (u8), 8);

    TEST_XY(sizeof (f4), 4);
    TEST_XY(sizeof (f8), 8);
}

int main (int argc, char** argv) {
	cerr << "TESTS COMPLETED SUCCESSFULLY!" << endl;
	return EXIT_SUCCESS;
}
