#include "Test.h"
#include "File.h"

TEST_CASE(TestFileExists) {
    File    file ("Utf8Test.txt");
    TEST_XY(file.getExists (), true);
}

