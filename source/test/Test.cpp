#include "Test.h"

/*
TEST_CASE(ForceFailure) {
    TEST_ASSERTION(false);
}
*/

UnitTest* UnitTest::currentUnitTest = 0;
BagObject* UnitTest::configurationObject;

int main (int argc, char** argv) {
	cerr << "TESTS COMPLETED SUCCESSFULLY!" << endl;

	UnitTest::writeConfiguration();

	return EXIT_SUCCESS;
}
