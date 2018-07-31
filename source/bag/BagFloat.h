#pragma once

#include    "BagThing.h"

MAKE_PTR_TO_SUB(BagFloat, BagThing) {
    protected:
        double value;

    public:
        BagFloat (double _value) : BagThing (BagThing::FLOAT_TYPE), value (_value) {}
        virtual ~BagFloat () {}
        virtual Text toText () const { return Text () << value; }
};
