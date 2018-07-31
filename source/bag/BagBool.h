#pragma once

#include    "BagThing.h"

MAKE_PTR_TO_SUB(BagBool, BagThing) {
    protected:
        bool value;

    public:
        BagBool (bool _value) : BagThing (BagThing::BOOL_TYPE), value (_value) {}
        virtual ~BagBool () {}
        virtual Text toText () const { return value ? Text::TRUE : Text ::FALSE; }
};
