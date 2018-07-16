#include "Test.h"

/*
TEST_CASE(ForceFailure) {
    TEST_ASSERTION(false);
}
*/

UnitTest* UnitTest::currentUnitTest = 0;

int main (int argc, char** argv) {
	cerr << "TESTS COMPLETED SUCCESSFULLY!" << endl << endl;
	return EXIT_SUCCESS;
}
