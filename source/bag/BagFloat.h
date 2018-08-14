#pragma once

#include "BagThing.h"

MAKE_PTR_TO_SUB(BagFloat, BagThing) {
    protected:
        f8 value;

    public:
        BagFloat (double _value) : BagThing (BagThing::FLOAT_TYPE), value (_value) {}
        virtual ~BagFloat () {}
        virtual Text toJson () const { return Text () << value; }
        virtual f8 sortValue () const { return value; };
        f8 get () { return value; }
};
