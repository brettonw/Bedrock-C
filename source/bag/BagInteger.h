#pragma once

#include "BagThing.h"

MAKE_PTR_TO_SUB(BagInteger, BagThing) {
    protected:
        s8 value;

    public:
        BagInteger (int _value) : BagThing (BagThing::INTEGER_TYPE), value (_value) {}
        virtual ~BagInteger () {}
        virtual Text toJson () const { return Text () << value; }
        virtual f8 sortValue () const { return static_cast<f8> (value); };
        s8 get () { return value; }
};
