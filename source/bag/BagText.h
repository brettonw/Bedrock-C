#pragma once

#include    "BagThing.h"

MAKE_PTR_TO_SUB(BagText, BagThing) {
    protected:
        Text value;

    public:
        BagText (const Text& _value) : BagThing (BagThing::TEXT_TYPE), value (_value) {}
        virtual ~BagText () { }
        virtual Text toJson () const { return enquote (toText ()); }
        virtual Text toText () const { return value; }
};
