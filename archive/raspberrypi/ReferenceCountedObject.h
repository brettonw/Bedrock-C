#pragma once

#include    "Types.h"

class ReferenceCountedObject {
    private:
        UInt    referenceCount;

        #ifndef NDEBUG
        UInt debugId;
        static UInt nextDebugId;
        #endif

    public:
        ReferenceCountedObject () : referenceCount (0) {
             #ifndef NDEBUG
            debugId = ++nextDebugId;
            #endif
        }

        ReferenceCountedObject (const ReferenceCountedObject& object) : referenceCount (0) {}

        virtual ~ReferenceCountedObject ();

        void addRef () {
            referenceCount++;
        }

        UInt removeRef () {
            return --referenceCount;
        }

        UInt getCount () const {
            return referenceCount;
        }
};

