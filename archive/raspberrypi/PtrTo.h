#pragma once

#include    "ReferenceCountedObject.h"

template <class aType>
class PtrTo {
    private:
        aType * ptrToType;

        static void addRefToPtr(ReferenceCountedObject* ptr) {
            if (ptr) {
                ptr->addRef();
            }
        }

        static void removeRefFromPtr(ReferenceCountedObject* ptr) {
            if (ptr and (ptr->removeRef() == 0)) {
                delete ptr;
            }
        }

    public:
        PtrTo () : ptrToType(0) {}

        PtrTo (aType* ptr) : ptrToType(ptr) {
            addRefToPtr(ptrToType);
        }

        PtrTo (const PtrTo<aType>& ptr) : ptrToType(ptr.ptrToType) {
            addRefToPtr(ptrToType);
        }

        virtual ~PtrTo () {
            removeRefFromPtr(ptrToType);
            ptrToType = 0;
        }

        const aType* getPtr () const {
            return ptrToType;
        }

        const aType& operator * () const {
            assertCall(ptrToType);
            return *ptrToType;
        }

        const aType* operator -> () const {
            return ptrToType;
        }

        aType* getPtr () {
            return ptrToType;
        }

        aType& operator * () {
            assertCall(ptrToType);
            return *ptrToType;
        }

        aType* operator -> () {
            return ptrToType;
        }

        operator const void* () const {
            return ptrToType;
        }

        PtrTo<aType>& operator = (aType* ptr) {
            addRefToPtr(ptr);
            removeRefFromPtr(ptrToType);
            ptrToType = ptr;
            return *this;
        }
        PtrTo<aType>& operator = (const PtrTo<aType>& ptr) {
            addRefToPtr(ptr.ptrToType);
            removeRefFromPtr(ptrToType);
            ptrToType = ptr.ptrToType;
            return *this;
        }

        // inequality and equality compare the *pointers*, so be careful not to expect
        // some other behavior
        Bool operator == (const PtrTo<aType>& ptr) {
            return ptrToType == ptr.ptrToType;
        }

        Bool operator != (const PtrTo<aType>& ptr) {
            return ptrToType != ptr.ptrToType;
        }

        Bool isUnique () const {
            // "null" is always unique, like NaN
            return ptrToType ? (ptrToType->getCount() == 1) : true;
        }

        void makeUnique () {
            if (not isUnique()) {
                aType* ptr = new aType(*ptrToType);
                addRefToPtr(ptr);
                removeRefFromPtr(ptrToType);
                ptrToType = ptr;
            }
        }
};

// a macro to help with defining pointers
#define MAKE_PTR_TO(aType) class aType; typedef PtrTo<aType> PtrTo ## aType; class aType
