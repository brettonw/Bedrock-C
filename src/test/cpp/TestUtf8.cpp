#include "Test.h"
#include "Utf8.h"

TEST_CASE(TestUtf8) {
    const char* str = "Hello World";
    Utf8    reader (str);
    while (*reader != 0) {
        int compare = *str++;
        TEST_XY(*reader, compare);
        ++reader;
    }
}
