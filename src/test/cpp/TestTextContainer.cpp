#include "Test.h"
#include "TextContainer.h"

TEST_CASE(TestTextLess) {
    Text    a ("hello");
    Text    b ("there");
    Text    c ("world");
    TextLess textLess;
    TEST_XY (textLess (a, b), true);
    TEST_XY (textLess (b, c), true);
    TEST_XY (textLess (a, c), true);
    TEST_XY (textLess (c, a), false);
}

TEST_CASE(TestTextSet) {
    TextSet ts;
    ts.put ("Hello");
    ts.put ("World");

    TEST_XY (ts.size(), 2);

    TEST_XY (ts.contains(Text ("JOE")), false);
    TEST_XY (ts.contains(Text ("Hello")), true);

    cerr << ts << endl;
}

TEST_CASE(TestTextMap) {
}
