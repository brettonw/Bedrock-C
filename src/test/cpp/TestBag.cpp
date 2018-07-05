#include "Test.h"
#include "Bag.h"

TEST_CASE(TestBag1) {
    PtrToBagArray bagArray = new BagArray ();
    bagArray->add (5);
    PtrToBagObject bagObject = new BagObject ();
    bagObject->put("xxx", bagArray);
    Text json = bagObject->toText ();
    cerr << json << endl;
}

TEST_CASE(TestBag2) {
    PtrToBagArray bagArray = (new BagArray ())->add (5)->add (true)->add ("hello")->add (1.665)->add ((new BagObject ())->put ("yyy", 15));
    PtrToBagObject bagObject = (new BagObject ())->put("xxx", bagArray);
    Text json = bagObject->toText ();
    cerr << json << endl;
}
