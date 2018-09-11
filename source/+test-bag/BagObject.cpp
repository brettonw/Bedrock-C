#include "Test.h"
#include "Bag.h"

TEST_CASE(TestEmptyBagObject) {
    PtrToBagObject bagObject = new BagObject ();
    TEST_EQUALS(bagObject->size (), 0);
    TEST_EQUALS(bagObject->toJson (), "{}");
}

TEST_CASE(TestBagObject) {
    //Log::Scope scope (Log::TRACE);
    PtrToBagObject bagObject = (new BagObject ())
        ->put ("A", "Hello World")
        ->put ("B", 5)
        ->put ("C", true)
        ->put ("DD", 6.5);
    TEST_EQUALS(bagObject->size (), 4);
    TEST_EQUALS(bagObject->toJson (), "{\"A\":\"Hello World\",\"B\":5,\"C\":true,\"DD\":6.50000000}");

    TEST_EQUALS(bagObject->get ("A")->toJson (), "\"Hello World\"");
    TEST_EQUALS(bagObject->get ("B")->toJson (), "5");
    TEST_EQUALS(bagObject->get ("C")->toJson (), "true");
    TEST_EQUALS(bagObject->get ("DD")->toJson (), "6.50000000");
    TEST_EQUALS(bagObject->get ("D"), (BagThing*) 0);

    TEST_EQUALS(bagObject->getText ("A"), "Hello World");
    TEST_EQUALS(bagObject->getInteger ("B"), 5);
    TEST_EQUALS(bagObject->getBool ("C"), true);
    TEST_EQUALS(bagObject->getFloat ("DD"), 6.5);

    TEST_EQUALS(bagObject->get ("A")->getType (), BagType::TEXT);
    TEST_EQUALS(bagObject->get ("B")->getType (), BagType::INTEGER);
    TEST_EQUALS(bagObject->get ("C")->getType (), BagType::BOOL);
    TEST_EQUALS(bagObject->get ("DD")->getType (), BagType::FLOAT);

    //bagObject->put ("XX", new BagArray ());
}

TEST_CASE(TestBag1) {
    PtrToBagArray bagArray = new BagArray ();
    bagArray->add (5);
    PtrToBagObject bagObject = new BagObject ();
    bagObject->put("xxx", bagArray);
    Text bag = bagObject->toJson ();
    TEST_EQUALS(bag, "{\"xxx\":[5]}");
    Log::debug () << bag << endl;
}

TEST_CASE(TestBag2) {
    PtrToBagArray bagArray = (new BagArray ())->add (5)->add (true)->add ("hello")->add (1.665)->add ((new BagObject ())->put ("yyy", 15));
    PtrToBagObject bagObject = (new BagObject ())->put("xxx", bagArray);
    Text bag = bagObject->toJson ();
    TEST_EQUALS(bag, "{\"xxx\":[5,true,\"hello\",1.66500000,{\"yyy\":15}]}");
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
    TEST_EQUALS(bagArray->size (), 5);
}
