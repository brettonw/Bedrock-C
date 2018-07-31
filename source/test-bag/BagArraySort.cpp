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
    PtrToBagArray sortedBagArray = BagArraySort::sort (bagArray);
    Log::debug () << "TestBagArraySort: " << "unsorted (" << bagArray->toText() << ")" << endl;
    Log::debug () << "TestBagArraySort: " << "sorted (" << sortedBagArray->toText () << ")" << endl;
    TEST_XY(bagArray->size(), sortedBagArray->size ());
    TEST_XY(bagArray->get(0)->sortValue(), 5);
    TEST_XY(sortedBagArray->get(0)->sortValue(), 0);
}
