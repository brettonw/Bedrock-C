// XXX TODO - the output is not "pretty", formatted, or otherwise made especially
// XXX TODO   human readable, though we could certainly do that.

#include    "TextContainer.h"

#pragma once

MAKE_PTR_TO(BagThing) {
    public:
        typedef enum {
            STR_TYPE,
            INT_TYPE,
            FLT_TYPE,
            BLN_TYPE,
            ARR_TYPE,
            OBJ_TYPE
        } Type;

		BagThing (Type _type) : type (_type) {}
        virtual ~BagThing () {}

        Type getType () const { return type; }
        virtual Text toText () const = 0;

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

MAKE_PTR_TO_SUB(BagText, BagThing) {
    protected:
        Text value;

    public:
        BagText (const Text& _value) : BagThing (STR_TYPE), value (_value) {}
        virtual ~BagText () { }
        virtual Text toText () const { return enquote (value); }
};

MAKE_PTR_TO_SUB(BagInteger, BagThing) {
    protected:
        int value;

    public:
        BagInteger (int _value) : BagThing (INT_TYPE), value (_value) {}
        virtual ~BagInteger () {}
        virtual Text toText () const { return Text () << value; }
};

MAKE_PTR_TO_SUB(BagFloat, BagThing) {
    protected:
        double value;

    public:
        BagFloat (double _value) : BagThing (FLT_TYPE), value (_value) {}
        virtual ~BagFloat () {}
        virtual Text toText () const { return Text () << value; }
};

MAKE_PTR_TO_SUB(BagBool, BagThing) {
    protected:
        bool value;

    public:
        BagBool (bool _value) : BagThing (BLN_TYPE), value (_value) {}
        virtual ~BagBool () {}
        virtual Text toText () const { return Text () << value; }
};

MAKE_PTR_TO_SUB(BagArray, BagThing) {
    protected:
        vector<PtrToBagThing> value;
        Text beginEnclosure;
        Text endEnclosure;
        Text separator;

        PtrToBagArray add (BagThing* bagThing) {
            value.push_back (bagThing);
            return this;
        }

    public:
        BagArray () : BagThing (ARR_TYPE), beginEnclosure ("["), endEnclosure ("]"), separator (",") {}
        BagArray (const Text& _begin, const Text& _end, const Text& _separator) : BagThing (ARR_TYPE), beginEnclosure (_begin), endEnclosure (_end), separator (_separator) {}
        virtual ~BagArray () {}

        virtual Text toText () const {
            Text prepend = "";
            Text out;
            for (vector<PtrToBagThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << (*it)->toText ();
                prepend = separator;
            }
            return enclose (out, beginEnclosure, endEnclosure);
        }

        template<typename BagThingSubtype>
        PtrToBagArray add (PtrTo<BagThingSubtype> bagThing) {
            value.push_back (ptr_upcast<BagThing>(bagThing));
            return this;
        }

        PtrToBagArray add(const char* thing) {
            return add (new BagText (thing));
        }

        PtrToBagArray add(int thing) {
            return add (new BagInteger (thing));
        }

        PtrToBagArray add(double thing) {
            return add (new BagFloat (thing));
        }

        PtrToBagArray add(bool thing) {
            return add (new BagBool (thing));
        }

        uint size () {
            return value.size();
        }

        PtrToBagThing get (uint index) {
            return (index < value.size ()) ? value[index] : (BagThing*)0;
        }

        const PtrToBagThing get (uint index) const {
            return (index < value.size ()) ? value[index] : (BagThing*)0;
        }

        // sort
        // "get" that takes a path string
};

MAKE_PTR_TO_SUB(BagObject, BagThing) {
    protected:
        TextMap<PtrToBagThing> value;

        PtrToBagObject put (const Text& name, BagThing* bagThing) {
            value.set(name, bagThing);
            return this;
        }

    public:
        BagObject () : BagThing (OBJ_TYPE) {}
        virtual ~BagObject () {}
        virtual Text toText () const {
            const char* prepend = "";
            Text out;
            for (TextMap<PtrToBagThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << enquote (it->first) << ":" << it->second->toText ();
                prepend = ",";
            }
            return enclose (out, "{", "}");
        }

        template<typename BagThingSubtype>
        PtrToBagObject put (const Text& name, PtrTo<BagThingSubtype> bagThing) {
            value.set(name, ptr_upcast<BagThing>(bagThing));
            return this;
        }

        PtrToBagObject put (const Text& name, const char* thing) {
            return put (name, new BagText (thing));
        }

        PtrToBagObject put (const Text& name, int thing) {
            return put (name, new BagInteger (thing));
        }

        PtrToBagObject put (const Text& name, double thing) {
            return put (name, new BagFloat (thing));
        }

        PtrToBagObject put (const Text& name, bool thing) {
            return put (name, new BagBool (thing));
        }

        uint size () {
            return value.size();
        }

        PtrToBagThing get (const Text& name) {
            PtrToBagThing* handle = value.get (name);
            return (handle) ? *handle : (BagThing*)0;
        }

        const PtrToBagThing get (const Text& name) const {
            const PtrToBagThing* handle = value.get (name);
            return (handle) ? *handle : (BagThing*)0;
        }

        // "get" that takes a path string
};

/*
CSV 		- array of objects,
			- index and sort functions

XML 		- Object with array of child objects, _tag, and _body (everything else is an attribute)
			- find function to return array of matching objects

JSON 		- whatever

Registry 	- Objects of Objects
			- findPath function
*/
