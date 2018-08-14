#include "Test.h"
#include "Bag.h"

TEST_CASE(TestBagThings) {
    PtrToBagText ptrToBagText = new BagText ("Hello World");
    PtrToBagInteger ptrToBagInteger = new BagInteger (5);
    PtrToBagBool ptrToBagBool = new BagBool (true);
    PtrToBagFloat ptrToBagFloat = new BagFloat (6.5);

    TEST_XY(ptrToBagText->toJson (), "\"Hello World\"");
    TEST_XY(ptrToBagText->get (), "Hello World");
    TEST_XY(ptrToBagInteger->toJson (), "5");
    TEST_XY(ptrToBagInteger->get (), 5);
    TEST_XY(ptrToBagBool->toJson (), "true");
    TEST_XY(ptrToBagBool->get (), true);
    TEST_XY(ptrToBagFloat->toJson (), "6.50000000");
    TEST_XY(ptrToBagFloat->get (), 6.5);

    TEST_XY(ptrToBagText->getType (), BagThing::TEXT_TYPE);
    TEST_XY(ptrToBagInteger->getType (), BagThing::INTEGER_TYPE);
    TEST_XY(ptrToBagBool->getType (), BagThing::BOOL_TYPE);
    TEST_XY(ptrToBagFloat->getType (), BagThing::FLOAT_TYPE);
}
