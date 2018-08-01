#include "Test.h"
#include "Bag.h"

TEST_CASE(TestEmptyBagArray) {
    PtrToBagArray bagArray = new BagArray ();
    TEST_XY(bagArray->size (), 0);
    TEST_XY(bagArray->toJson (), "[]");
    TEST_XY(bagArray->toText (), "[]");
}

TEST_CASE(TestBagArray) {
    PtrToBagArray bagArray = (new BagArray ())
        ->add ("Hello World")
        ->add (5)
        ->add (true)
        ->add (6.5);
    TEST_XY(bagArray->size (), 4);
    TEST_XY(bagArray->toJson (), "[\"Hello World\",5,true,6.50000000]");
    TEST_XY(bagArray->toText (), "[Hello World,5,true,6.50000000]");

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

TEST_CASE(TestBagArraySimpleSort) {
    Log::Scope scope (Log::DEBUG);
    PtrToBagArray bagArray = new BagArray ();
    bagArray->add(5);
    bagArray->add(false);
    bagArray->add(5.6);
    bagArray->add("4.5");
    bagArray->add(4);
    bagArray->add(4.0);
    bagArray->add(true);
    bagArray->add(0.5);
    bagArray->add("0.75");
    Log::debug () << "TestBagArraySort: " << "unsorted (" << bagArray->toText() << ")" << endl;
    TEST_XY(bagArray->get(0)->sortValue(), 5);

    PtrToBagArray sortedBagArray = BagArray::sort (bagArray);
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    TEST_XY(sortedBagArray->get(0)->sortValue(), 0);

    sortedBagArray = BagArray::sort (bagArray, DESCENDING);
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    TEST_XY(sortedBagArray->get(0)->sortValue(), 5.6);
}

TEST_CASE(TestBagArrayFieldSort) {
    Log::Scope scope (Log::DEBUG);
    PtrToBagArray bagArray = new BagArray ();
    bagArray->add((new BagObject ())->put ("x", 5));
    bagArray->add((new BagObject ())->put ("x", false));
    bagArray->add((new BagObject ())->put ("x", 5.6));
    bagArray->add((new BagObject ())->put ("x", "4.5"));
    bagArray->add((new BagObject ())->put ("x", 4));
    bagArray->add((new BagObject ())->put ("x", 4.0));
    bagArray->add((new BagObject ())->put ("x", true));
    bagArray->add((new BagObject ())->put ("x", 0.5));
    bagArray->add((new BagObject ())->put ("x", "0.75"));
    Log::debug () << "TestBagArraySort: " << "unsorted (" << bagArray->toText() << ")" << endl;
    //TEST_XY(bagArray->get(0)->sortValue(), 5);

    PtrToBagArray sortedBagArray = BagArray::sort (bagArray, "x");
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    //TEST_XY(sortedBagArray->get(0)->sortValue(), 0);

    sortedBagArray = BagArray::sort (bagArray, "x", DESCENDING);
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    //TEST_XY(sortedBagArray->get(0)->sortValue(), 5.6);
}

TEST_CASE(TestBagArrayDbSort) {
    Log::Scope scope (Log::DEBUG);
    PtrToBagArray bagArray = new BagArray ();
    bagArray->add((new BagObject ())->put ("x", 5)->put ("y", 1));
    bagArray->add((new BagObject ())->put ("x", 5)->put ("y", 2));
    bagArray->add((new BagObject ())->put ("x", 5)->put ("y", 3));
    bagArray->add((new BagObject ())->put ("x", 4)->put ("y", 1));
    bagArray->add((new BagObject ())->put ("x", 4)->put ("y", 2));
    bagArray->add((new BagObject ())->put ("x", 4)->put ("y", 3));
    bagArray->add((new BagObject ())->put ("x", 3)->put ("y", 1));
    bagArray->add((new BagObject ())->put ("x", 3)->put ("y", 2));
    bagArray->add((new BagObject ())->put ("x", 3)->put ("y", 3));
    Log::debug () << "TestBagArraySort: " << "unsorted (" << bagArray->toText() << ")" << endl;
    //TEST_XY(bagArray->get(0)->sortValue(), 5);

    PtrToBagArray sortedBagArray = BagArray::sort (bagArray, DbSort ("x")("y", DESCENDING));
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    //TEST_XY(sortedBagArray->get(0)->sortValue(), 0);

    sortedBagArray = BagArray::sort (bagArray, DbSort ("y")("x", DESCENDING));
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    //TEST_XY(sortedBagArray->get(0)->sortValue(), 5.6);
}
