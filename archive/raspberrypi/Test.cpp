#include    "PtrTo.h"

using namespace std;

queue<Bool>	expect;
void setExpect (Bool e) {
    expect.push (e);
}

Bool popExpect () {
	if (expect.size () > 0) {
		Bool result = expect.front ();
		expect.pop ();
		return result;
	} else {
		return true;
	}
}

void testAssert (CString string, Bool condition, CString file, SInt line) {
	Bool expectCondition = popExpect ();
    if (condition != expectCondition) {
        cerr << "FAIL: " << file << ", " << "line " << line << " -> [(" << string << ") == " << (expectCondition ? "true" : "false") << "]" << endl;
        throw "failed assertion";
    }
    cerr << "PASS: " << file << ", " << "line " << line << " -> [(" << string << ") == " << (expectCondition ? "true" : "false") << "]" << endl;
}

//-------------------------------------------------------------------------------------------------
// The example here shows how to create a class, "TestClass" that derives from the reference
// counted type, and use the macro "MAKE_PTR_TO" to implicitly create the type-safe pointer type,
// "PtrToTestClass". TestClass overrides the "new" and "delete" operators for test purposes, it is
// not a requirement that reference counted objects do that.
//-------------------------------------------------------------------------------------------------
MAKE_PTR_TO(TestClass) : public ReferenceCountedObject {
    private:
        SInt a;
        static	set<void*>	ptrs;

    public:
            TestClass (SInt a) {
                this->a = a;
            }
            ~TestClass() {}

            SInt get () const { return a; }

		    void* operator new (size_t size) {
                void* ptr = malloc(size);
                cerr << "new: " << ptr << "\n";
                ptrs.insert(ptr);
                return ptr;
            }

		    void operator delete (void* ptr) {
                cerr << "delete: " << ptr << "\n";
                assertCall(ptrs.find(ptr) != ptrs.end());
                ptrs.erase(ptr);
                free(ptr);
            }

            static void validate (UInt expectedCount = 0) {
                set<void*>::iterator it;
                for (set<void*>::iterator it = ptrs.begin(); it != ptrs.end(); ++it) {
                    cerr << "outstanding pointer: " << *it << "\n";
                }
                assertCall(ptrs.size() == expectedCount);
            }
};

set<void*> TestClass::ptrs;

// shows passing the pointer by reference, be careful doing this
// but this is normal usage if the function won't take ownership
// of the pointer (i.e. won't store it before returning)
void testPtrToParameter (const PtrToTestClass& ptrToTestClass) {
    assertCall (ptrToTestClass->getCount () == 1);
    assertCall (ptrToTestClass.isUnique ());
    assertCall (ptrToTestClass->get () == 5);
}

// shows passing the pointer by value, and implicitly constructing 
// a new pointer, this should be the normal usage for a function
// that takes ownership of the ptr (i.e. it will store the ptr in
// another data structure for later use)
void testPtrToParameter2 (PtrToTestClass ptrToTestClass) {
    assertCall (ptrToTestClass->getCount () == 2);
    setExpect (false);
    assertCall (ptrToTestClass.isUnique ());
    assertCall (ptrToTestClass->get () == 5);
}

void testPtrTo () {
    // test basic functions
    PtrToTestClass ptrToTestClass;
    assertCall (ptrToTestClass.getPtr () == 0);
    assertCall (ptrToTestClass.isUnique ());

    ptrToTestClass = new TestClass (5);
    assertCall (ptrToTestClass.getPtr () != 0);
    assertCall (ptrToTestClass->getCount () == 1);
    assertCall (ptrToTestClass.isUnique ());
    assertCall (ptrToTestClass->get () == 5);

    testPtrToParameter (ptrToTestClass);
    testPtrToParameter2 (ptrToTestClass);

    // test within a block
    PtrToTestClass* ptrToPtr;
    if (true) {
        if (true) {
            PtrToTestClass anotherPtr = ptrToTestClass;
            assertCall (anotherPtr.getPtr () != 0);
            assertCall (anotherPtr->getCount () == 2);
            assertCall (anotherPtr.isUnique () == false);
            assertCall (anotherPtr->get () == 5);
            assertCall (ptrToTestClass.getPtr () != 0);
            assertCall (ptrToTestClass->getCount () == 2);
            assertCall (ptrToTestClass.isUnique () == false);
            assertCall (ptrToTestClass->get () == 5);
        }

        assertCall (ptrToTestClass.getPtr () != 0);
        assertCall (ptrToTestClass->getCount () == 1);
        assertCall (ptrToTestClass.isUnique ());
        assertCall (ptrToTestClass->get () == 5);

        PtrToTestClass anotherPtr = new TestClass (6);
        ptrToPtr = &anotherPtr;
    }

    // oooh risky (waves hands dramatically) - checking memory 
    // that has gone out of scope for an expected value
    assertCall ((*ptrToPtr).getPtr () == 0);
}

SInt main (SInt argc, String* argv) {
	try {
		testPtrTo ();

        // make sure there is no dangling memory, as all the 
        // pointers went out of scope and should have been 
        // cleaned up...
		TestClass::validate ();
		cerr << "OVERALL: PASS\n";
        return EXIT_SUCCESS;
    } catch (const char* exception) {
    	cerr << "OVERALL: FAIL: " << exception << "\n";
        return EXIT_FAILURE;
    }
}
