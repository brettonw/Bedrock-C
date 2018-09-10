#include "Test.h"
#include "Bag.h"

TEST_CASE(TestBagThings) {
    PtrToBagText ptrToBagText = new BagText ("Hello World");
    PtrToBagInteger ptrToBagInteger = new BagInteger (5);
    PtrToBagBool ptrToBagBool = new BagBool (true);
    PtrToBagFloat ptrToBagFloat = new BagFloat (6.5);

    TEST_EQUALS(ptrToBagText->toJson (), "\"Hello World\"");
    TEST_EQUALS(ptrToBagText->get (), "Hello World");
    TEST_EQUALS(ptrToBagInteger->toJson (), "5");
    TEST_EQUALS(ptrToBagInteger->get (), 5);
    TEST_EQUALS(ptrToBagBool->toJson (), "true");
    TEST_EQUALS(ptrToBagBool->get (), true);
    TEST_EQUALS(ptrToBagFloat->toJson (), "6.50000000");
    TEST_EQUALS(ptrToBagFloat->get (), 6.5);

    TEST_EQUALS(ptrToBagText->getType (), BagThing::TEXT_TYPE);
    TEST_EQUALS(ptrToBagInteger->getType (), BagThing::INTEGER_TYPE);
    TEST_EQUALS(ptrToBagBool->getType (), BagThing::BOOL_TYPE);
    TEST_EQUALS(ptrToBagFloat->getType (), BagThing::FLOAT_TYPE);
}
