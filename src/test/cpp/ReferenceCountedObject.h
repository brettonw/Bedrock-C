#include "UNIT_TEST/unit_tester.H"
#include "ReferenceCountedObject.H"


BOOST_AUTO_TEST_CASE(TestReferenceCountedObject) {
    // base object constructor, addRef, and geCount
    ReferenceCountedObject  rcObject;
    TESTXY(rcObject.getReferenceCount (), 0);
    rcObject.addRef ();
    TESTXY(rcObject.getReferenceCount (), 1);
    rcObject.addRef ();
    TESTXY(rcObject.getReferenceCount (), 2);

    // remove ref
    rcObject.removeRef ();
    TESTXY(rcObject.getReferenceCount (), 1);

    // copy constructor
    ReferenceCountedObject rcObject2 (rcObject);
    TESTXY(rcObject2.getReferenceCount (), 0);

    // finish and clean up
    rcObject.removeRef ();
    TESTXY(rcObject.getReferenceCount (), 0);

    // don't bother to formally test the debug id
}
