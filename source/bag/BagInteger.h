#pragma once

#include    "BagThing.h"

MAKE_PTR_TO_SUB(BagInteger, BagThing) {
    protected:
        int value;

    public:
        BagInteger (int _value) : BagThing (BagThing::INTEGER_TYPE), value (_value) {}
        virtual ~BagInteger () {}
        virtual Text toText () const { return Text () << value; }
};
