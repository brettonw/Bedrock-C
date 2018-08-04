#include "Test.h"
#include "DigitOp.h"

TEST_CASE(TestDigitOpTop) {
    Log::Scope scope (Log::DEBUG);

    u1  a[] = {0x00, 0x00, 0x00};
    uint top = DigitOp<u1, 3>::top (a);
    TEST_XY(top, 0);

    u1  b[] = {0x01, 0x00, 0x00};
    top = DigitOp<u1, 3>::top (b);
    TEST_XY(top, 1);

    u1  c[] = {0x00, 0x01, 0x00};
    top = DigitOp<u1, 3>::top (c);
    TEST_XY(top, 2);

    u1  d[] = {0x00, 0x00, 0x01};
    top = DigitOp<u1, 3>::top (d);
    TEST_XY(top, 3);
}
