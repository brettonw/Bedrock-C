#include "UNIT_TEST/unit_tester.H"
#include "Text.H"

BOOST_AUTO_TEST_CASE(TestText0) {
    TESTXY(sizeof(Text), sizeof(void*));
}

BOOST_AUTO_TEST_CASE(TestText1) {
    const char* hello = "Hello";
    const char* world = "World";
    Text helloText = hello;
    TESTXYOP(helloText, hello, ==);
    TESTXY(helloText.length(), 5);

    Text worldText (world);
    TESTXYOP(worldText, world, ==);
    TESTXY(worldText.length(), 5);

    TESTXY(helloText.compare(worldText), 'H' - 'W');
    TESTXY(worldText.compare(helloText), 'W' - 'H');
    TESTXY(helloText < worldText, true);
    TESTXY(helloText <= worldText, true);
    TESTXY(worldText > helloText, true);
    TESTXY(worldText >= helloText, true);
    TESTXY(helloText == helloText, true);
    TESTXY(helloText != helloText, false);
    TESTXY(helloText == "", false);
    TESTXY(helloText != "", true);
    TESTXY(helloText != 0, true);

    Text helloWorldText = helloText + " " + worldText;
    TESTXYOP(helloWorldText, "Hello World", ==);
    TESTXY(helloWorldText.length(), 11);

    helloWorldText.clear ();
    TESTXY(helloWorldText.length(), 0);

    helloWorldText.clear () << world << "-" << hello;
    TESTXYOP(helloWorldText, "Hello World", !=);
    TESTXYOP(helloWorldText, "World-Hello", ==);

    helloWorldText.clear () += worldText;
    TESTXYOP(helloWorldText, "Hello World", !=);
    TESTXYOP(helloWorldText, "World", ==);

    Text emptyText;
    TESTXYOP(emptyText, "", ==);
}
