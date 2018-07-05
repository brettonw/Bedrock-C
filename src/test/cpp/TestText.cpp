#include "Test.h"
#include "Text.h"

TEST_CASE(TestText0) {
    TEST_XY(sizeof(Text), sizeof(void*));
}

TEST_CASE(TestText1) {
    const char* hello = "Hello";
    const char* world = "World";
    Text helloText = hello;
    TEST_XYOP(helloText, hello, ==);
    TEST_XY(helloText.length(), 5);

    Text worldText (world);
    TEST_XYOP(worldText, world, ==);
    TEST_XY(worldText.length(), 5);

    TEST_XY(helloText.compare(worldText), 'H' - 'W');
    TEST_XY(worldText.compare(helloText), 'W' - 'H');
    TEST_XY(helloText < worldText, true);
    TEST_XY(helloText <= worldText, true);
    TEST_XY(worldText > helloText, true);
    TEST_XY(worldText >= helloText, true);
    TEST_XY(helloText == helloText, true);
    TEST_XY(helloText != helloText, false);
    TEST_XY(helloText == "", false);
    TEST_XY(helloText != "", true);
    TEST_XY(helloText != 0, true);

    Text helloWorldText = helloText + " " + worldText;
    TEST_XYOP(helloWorldText, "Hello World", ==);
    TEST_XY(helloWorldText.length(), 11);

    helloWorldText.clear ();
    TEST_XY(helloWorldText.length(), 0);

    helloWorldText.clear () << world << "-" << hello;
    TEST_XYOP(helloWorldText, "Hello World", !=);
    TEST_XYOP(helloWorldText, "World-Hello", ==);

    helloWorldText.clear () += worldText;
    TEST_XYOP(helloWorldText, "Hello World", !=);
    TEST_XYOP(helloWorldText, "World", ==);

    Text emptyText;
    TEST_XYOP(emptyText, "", ==);
}
