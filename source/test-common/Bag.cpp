#include "Test.h"
#include "Bag.h"

TEST_CASE(TestBagThings) {
    PtrToBagText ptrToBagText = new BagText ("Hello World");
    PtrToBagInteger ptrToBagInteger = new BagInteger (5);
    PtrToBagBool ptrToBagBool = new BagBool (true);
    PtrToBagFloat ptrToBagFloat = new BagFloat (6.5);

    TEST_XY(ptrToBagText->toJson (), "\"Hello World\"");
    TEST_XY(ptrToBagText->toText (), "Hello World");
    TEST_XY(ptrToBagInteger->toJson (), "5");
    TEST_XY(ptrToBagInteger->toText (), "5");
    TEST_XY(ptrToBagBool->toJson (), "true");
    TEST_XY(ptrToBagBool->toText (), "true");
    TEST_XY(ptrToBagFloat->toJson (), "6.50000000");
    TEST_XY(ptrToBagFloat->toText (), "6.50000000");

    TEST_XY(ptrToBagText->getType (), BagThing::TEXT_TYPE);
    TEST_XY(ptrToBagInteger->getType (), BagThing::INTEGER_TYPE);
    TEST_XY(ptrToBagBool->getType (), BagThing::BOOL_TYPE);
    TEST_XY(ptrToBagFloat->getType (), BagThing::FLOAT_TYPE);
}

TEST_CASE(TestEmptyBagArr) {
    PtrToBagArray bagArray = new BagArray ();
    TEST_XY(bagArray->size (), 0);
    TEST_XY(bagArray->toJson (), "[]");
    TEST_XY(bagArray->toText (), "\n");
}

TEST_CASE(TestBagArr) {
    PtrToBagArray bagArray = (new BagArray ())
        ->add ("Hello World")
        ->add (5)
        ->add (true)
        ->add (6.5);
    TEST_XY(bagArray->size (), 4);
    TEST_XY(bagArray->toJson (), "[\"Hello World\",5,true,6.50000000]");
    TEST_XY(bagArray->toText (), "Hello World\n5\ntrue\n6.50000000\n");

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

TEST_CASE(TestEmptyBagObj) {
    PtrToBagObject bagObject = new BagObject ();
    TEST_XY(bagObject->size (), 0);
    TEST_XY(bagObject->toJson (), "{}");
    TEST_XY(bagObject->toText (), "\n");
}

TEST_CASE(TestBagObj) {
    //Log::Scope scope (Log::TRACE);
    PtrToBagObject bagObject = (new BagObject ())
        ->put ("A", "Hello World")
        ->put ("B", 5)
        ->put ("C", true)
        ->put ("DD", 6.5);
    TEST_XY(bagObject->size (), 4);
    TEST_XY(bagObject->toJson (), "{\"A\":\"Hello World\",\"B\":5,\"C\":true,\"DD\":6.50000000}");
    TEST_XY(bagObject->toText (), "A=Hello World\nB=5\nC=true\nDD=6.50000000\n");

    TEST_XY(bagObject->get ("A")->toJson (), "\"Hello World\"");
    TEST_XY(bagObject->get ("B")->toText (), "5");
    TEST_XY(bagObject->get ("C")->toText (), "true");
    TEST_XY(bagObject->get ("DD")->toText (), "6.50000000");
    TEST_XY(bagObject->get ("D"), (BagThing*) 0);

    TEST_XY(bagObject->get ("A")->getType (), BagThing::TEXT_TYPE);
    TEST_XY(bagObject->get ("B")->getType (), BagThing::INTEGER_TYPE);
    TEST_XY(bagObject->get ("C")->getType (), BagThing::BOOL_TYPE);
    TEST_XY(bagObject->get ("DD")->getType (), BagThing::FLOAT_TYPE);
}

TEST_CASE(TestBag1) {
    PtrToBagArray bagArray = new BagArray ();
    bagArray->add (5);
    PtrToBagObject bagObject = new BagObject ();
    bagObject->put("xxx", bagArray);
    Text bag = bagObject->toJson ();
    TEST_XY(bag, "{\"xxx\":[5]}");
    Log::debug () << bag << endl;
}

TEST_CASE(TestBag2) {
    PtrToBagArray bagArray = (new BagArray ())->add (5)->add (true)->add ("hello")->add (1.665)->add ((new BagObject ())->put ("yyy", 15));
    PtrToBagObject bagObject = (new BagObject ())->put("xxx", bagArray);
    Text bag = bagObject->toJson ();
    TEST_XY(bag, "{\"xxx\":[5,true,\"hello\",1.66500000,{\"yyy\":15}]}");
    Log::debug () << bag << endl;
}
