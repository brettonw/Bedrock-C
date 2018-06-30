#include "UNIT_TEST/unit_tester.H"
#include "TextContainer.H"


BOOST_AUTO_TEST_CASE(TestTextLess) {
    Text    a ("hello");
    Text    b ("there");
    Text    c ("world");
    TextLess textLess;
    TESTXY (textLess (a, b), true);
    TESTXY (textLess (b, c), true);
    TESTXY (textLess (a, c), true);
    TESTXY (textLess (c, a), false);
}

BOOST_AUTO_TEST_CASE(TestTextSet) {
    TextSet ts;
    ts.put ("Hello");
    ts.put ("World");

    TESTXY (ts.size(), 2);

    TESTXY (ts.contains(Text ("JOE")), false);
    TESTXY (ts.contains(Text ("Hello")), true);

    cerr << ts << endl;
}

BOOST_AUTO_TEST_CASE(TestTextMap) {
}
