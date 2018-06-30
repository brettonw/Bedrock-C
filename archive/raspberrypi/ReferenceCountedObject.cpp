#include    "ReferenceCountedObject.h"

#ifndef NDEBUG
UInt ReferenceCountedObject::nextDebugId = 0;
#endif

ReferenceCountedObject::~ReferenceCountedObject () {
	assertCall (referenceCount == 0);
}
