#pragma once

#include "BagThing.h"

MAKE_PTR_TO_SUB(BagText, BagThing) {
    protected:
        Text value;

    public:
        BagText (const Text& _value) : BagThing (BagThing::TEXT_TYPE), value (_value) {}
        virtual ~BagText () { }
        virtual Text toJson () const { return enquote (value); }
        virtual double sortValue () const { return strtod (value.get(), 0); };
        const Text& get () { return value; }
};
