#include "Test.h"
#include "Bag.h"

TEST_CASE(TestEmptyBagArray) {
    PtrToBagArray bagArray = new BagArray ();
    TEST_XY(bagArray->size (), 0);
    TEST_XY(bagArray->toJson (), "[]");
    TEST_XY(bagArray->toText (), "");
}

TEST_CASE(TestBagArray) {
    PtrToBagArray bagArray = (new BagArray ())
        ->add ("Hello World")
        ->add (5)
        ->add (true)
        ->add (6.5);
    TEST_XY(bagArray->size (), 4);
    TEST_XY(bagArray->toJson (), "[\"Hello World\",5,true,6.50000000]");
    TEST_XY(bagArray->toText (), "Hello World, 5, true, 6.50000000");

    TEST_XY(bagArray->get (0)->toJson (), "\"Hello World\"");
    TEST_XY(bagArray->get (0)->toText (), "Hello World");
    TEST_XY(bagArray->get (1)->toText (), "5");
    TEST_XY(bagArray->get (2)->toText (), "true");
    TEST_XY(bagArray->get (3)->toText (), "6.50000000");
    TEST_XY(bagArray->get (4), (BagThing*) 0);

    TEST_XY(bagArray->get (0)->getType (), BagThing::TEXT_TYPE);
    TEST_XY(bagArray->get (1)->getType (), BagThing::INTEGER_TYPE);
    TEST_XY(bagArray->get (2)->getType (), BagThing::BOOL_TYPE);
    TEST_XY(bagArray->get (3)->getType (), BagThing::FLOAT_TYPE);
}
