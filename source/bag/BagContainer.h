#pragma once

#include    "BagText.h"
#include    "BagInteger.h"
#include    "BagFloat.h"
#include    "BagBool.h"

MAKE_PTR_TO_SUB(BagContainer, BagThing) {
    public:
        BagContainer (Type _type) : BagThing (_type) {}

        virtual double sortValue () const { return 0.0; };

        PtrToBagThing get (const Text& path) {
            return const_cast<const BagContainer*>(this)->get (path);
        }

        virtual const PtrToBagThing get (const Text& path) const = 0;
};
