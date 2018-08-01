#include "Test.h"
#include "Bag.h"
#include "BagArraySort.h"

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

    PtrToBagArray sortedBagArray = BagArraySort::sort (bagArray);
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    TEST_XY(sortedBagArray->get(0)->sortValue(), 0);

    sortedBagArray = BagArraySort::sort (bagArray, DESCENDING);
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

    PtrToBagArray sortedBagArray = BagArraySort::sort (bagArray, "x");
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    //TEST_XY(sortedBagArray->get(0)->sortValue(), 0);

    sortedBagArray = BagArraySort::sort (bagArray, "x", DESCENDING);
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

    PtrToBagArray sortedBagArray = BagArraySort::sort (bagArray, BagArraySort::DbSort);
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    //TEST_XY(sortedBagArray->get(0)->sortValue(), 0);

    sortedBagArray = BagArraySort::sort (bagArray, "x", DESCENDING);
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    //TEST_XY(sortedBagArray->get(0)->sortValue(), 5.6);
}
