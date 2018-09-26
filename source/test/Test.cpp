#include "Test.h"

/*
TEST_CASE(ForceFailure) {
    TEST_ASSERTION(false);
}
*/

int main (int argc, char** argv) {
	cerr << "TESTS COMPLETED SUCCESSFULLY!" << endl;

	UnitTest::writeConfiguration();

	return EXIT_SUCCESS;
}
