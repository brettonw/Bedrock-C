#include "Test.h"
#include "File.h"

TEST_CASE(TestFileExists) {
    File    file ("Utf8Test.txt");
    TEST_XY(file.getExists (), true);
    TEST_XY(file.isDirectory (), false);

    File    directory ("..");
    TEST_XY(directory.getExists (), true);
    TEST_XY(directory.isDirectory (), true);

    File    junk ("nonexistent-file.txt");
    TEST_XY(junk.getExists (), false);
    TEST_XY(junk.isDirectory (), false);
}

