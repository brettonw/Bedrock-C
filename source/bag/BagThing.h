#pragma once

#include "Text.h"

MAKE_PTR_TO(BagThing) {
    public:
    typedef enum {
        TEXT_TYPE,
        INTEGER_TYPE,
        FLOAT_TYPE,
        BOOL_TYPE,
        ARRAY_TYPE,
        OBJECT_TYPE
    } Type;

        BagThing (Type _type) : type (_type) {}
        virtual ~BagThing () {}

        Type getType () const { return type; };
        virtual Text toJson () const = 0;
        virtual double sortValue () const = 0;

    protected:
        Type type;

        static Text enclose (const Text& value, const Text& beginEnclosure, const Text& endEnclosure) {
            return Text () << beginEnclosure << value << endEnclosure;
        }

        static Text enclose (const Text& value, const Text& enclosure) {
            return enclose (value, enclosure, enclosure);
        }

        static Text enquote (const Text& value) {
            return enclose (value, "\"");
        }
};
