#include "Test.h"
#include "Bag.h"

TEST_CASE(TestEmptyBagObject) {
    PtrToBagObject bagObject = new BagObject ();
    TEST_XY(bagObject->size (), 0);
    TEST_XY(bagObject->toJson (), "{}");
    TEST_XY(bagObject->toText (), "{}");
}

TEST_CASE(TestBagObject) {
    //Log::Scope scope (Log::TRACE);
    PtrToBagObject bagObject = (new BagObject ())
        ->put ("A", "Hello World")
        ->put ("B", 5)
        ->put ("C", true)
        ->put ("DD", 6.5);
    TEST_XY(bagObject->size (), 4);
    TEST_XY(bagObject->toJson (), "{\"A\":\"Hello World\",\"B\":5,\"C\":true,\"DD\":6.50000000}");
    TEST_XY(bagObject->toText (), "{A=Hello World,B=5,C=true,DD=6.50000000}");

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

TEST_CASE(TestBagObjectNameTable) {
    //Log::Scope scope (Log::TRACE);
    PtrToBagArray bagArray = new BagArray ();
    Text    def ("DEF");
    for (int i = 0; i < 5; ++i) {
        PtrToBagObject bagObject = (new BagObject ())
            ->put ("ABC", "123")
            ->put (def, "234")
            ->put ("GHI", "345")
            ->put ("JKL", "456")
            ->put ("MNO", "567");
        bagArray->add (bagObject);
    }
    TEST_XYOP(def.getReferenceCount(), 5, <);
    TEST_XY(bagArray->size (), 5);
}
