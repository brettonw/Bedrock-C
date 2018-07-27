#pragma once

// XXX TODO - the output is not "pretty", formatted, or otherwise made especially
// XXX TODO   human readable, though we could certainly do that.

#include    "Log.h"
#include    "TextContainer.h"

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
        virtual Text toJson () const { return toText (); };
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
        BagText (const Text& _value) : BagThing (BagThing::TEXT_TYPE), value (_value) {}
        virtual ~BagText () { }
        virtual Text toJson () const { return enquote (toText ()); }
        virtual Text toText () const { return value; }
};

MAKE_PTR_TO_SUB(BagInteger, BagThing) {
    protected:
        int value;

    public:
        BagInteger (int _value) : BagThing (BagThing::INTEGER_TYPE), value (_value) {}
        virtual ~BagInteger () {}
        virtual Text toText () const { return Text () << value; }
};

MAKE_PTR_TO_SUB(BagFloat, BagThing) {
    protected:
        double value;

    public:
        BagFloat (double _value) : BagThing (BagThing::FLOAT_TYPE), value (_value) {}
        virtual ~BagFloat () {}
        virtual Text toText () const { return Text () << value; }
};

MAKE_PTR_TO_SUB(BagBool, BagThing) {
    protected:
        bool value;

    public:
        BagBool (bool _value) : BagThing (BagThing::BOOL_TYPE), value (_value) {}
        virtual ~BagBool () {}
        virtual Text toText () const { return Text () << value; }
};

MAKE_PTR_TO_SUB(BagContainer, BagThing) {
    public:
        BagContainer (Type _type) : BagThing (_type) {}

        PtrToBagThing get (const Text& path) {
            return const_cast<const BagContainer*>(this)->get (path);
        }

        virtual const PtrToBagThing get (const Text& path) const = 0;
};

MAKE_PTR_TO_SUB(BagArray, BagContainer) {
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
        BagArray () : BagContainer (BagThing::ARRAY_TYPE), beginEnclosure ("["), endEnclosure ("]"), separator (",") {}
        BagArray (const Text& _begin, const Text& _end, const Text& _separator) : BagContainer (BagThing::ARRAY_TYPE), beginEnclosure (_begin), endEnclosure (_end), separator (_separator) {}
        virtual ~BagArray () {}

        virtual Text toJson () const {
            Text prepend = "";
            Text out;
            for (vector<PtrToBagThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << (*it)->toJson ();
                prepend = separator;
            }
            return enclose (out, beginEnclosure, endEnclosure);
        }

        virtual Text toText () const {
            Text prepend = "";
            Text out;
            for (vector<PtrToBagThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << (*it)->toText ();
                prepend = END_LINE;
            }
            return out << END_LINE;
        }

        template<typename BagThingSubtype>
        PtrToBagArray add (PtrTo<BagThingSubtype> bagThing) {
            value.push_back (ptr_upcast<BagThing>(bagThing));
            return this;
        }

        PtrToBagArray add(const Text& thing) {
            return add (new BagText (thing));
        }

        // old C++ - we either add this explicit conversion, or remove the
        // overload that handles bool types, as the compiler implicitly
        // chooses that conversion over this one
        PtrToBagArray add(const char* thing) {
            return add (Text (thing));
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

        virtual const PtrToBagThing get (const Text& path) const {
            vector<Text> parts = path.splitFirst ("/");
            uint index = atoi (parts[0].get ());
            PtrToBagThing ptrToBagThing = get (index);
            if ((parts.size () == 2) and ptrToBagThing) {
                switch (ptrToBagThing->getType ()) {
                    case BagThing::OBJECT_TYPE:
                    case BagThing::ARRAY_TYPE:
                        return ptr_downcast<BagContainer> (ptrToBagThing)->get (parts[1]);
                    default:
                        return (BagThing*) 0;
                }
            }
            return ptrToBagThing;
        }

        /*
        PtrToBagThing operator [] (uint index) {
            return get (index);
        }

        const PtrToBagThing operator [] (uint index) const {
            return get (index);
        }
        */
};

MAKE_PTR_TO_SUB(BagObject, BagContainer) {
    private:
        static TextMap<Text>* nameTable;

    protected:
        TextMap<PtrToBagThing> value;

        PtrToBagObject put (const Text& name, BagThing* bagThing) {
            // look up the requested name in our name table to check to see if we already have a
            // pointer to an identical string - this will mitigate repeated copying of the same
            // name, and reduce the memory footprint of BagObjects in aggregate.
            static TextMap<Text> nameTable;
            const Text* foundName = nameTable.get(name);
            Text useName = (foundName != 0) ? *foundName : (nameTable[name] = name);
            Log::trace () << "BagObject: " << "nameTable contains " << nameTable.size () << " reference" << ((nameTable.size () != 1 ) ? "s" : "") << endl;
            /*
            if (foundName != 0) {
                // we've already got a copy of this string - use that one
                useName = *foundName;
            } else {
                // we don't already have a copy, save this one
                nameTable[name] = useName = name;
            }
            */

            // and now actually assign the thing...
            value[useName] = bagThing;
            return this;
        }

    public:
        BagObject () : BagContainer (BagThing::OBJECT_TYPE) {}
        virtual ~BagObject () {}
        virtual Text toJson () const {
            const char* prepend = "";
            Text out;
            for (TextMap<PtrToBagThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << enquote (it->first) << ":" << it->second->toJson ();
                prepend = ",";
            }
            return enclose (out, "{", "}");
        }

        virtual Text toText () const {
            const char* prepend = "";
            Text out;
            for (TextMap<PtrToBagThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << it->first << "=" << it->second->toText ();
                prepend = END_LINE;
            }
            return out << END_LINE;
        }

        template<typename BagThingSubtype>
        PtrToBagObject put (const Text& name, PtrTo<BagThingSubtype> bagThing) {
            return put (name, bagThing.getPtr ());
        }

        PtrToBagObject put (const Text& name, const Text& thing) {
            return put (name, new BagText (thing));
        }

        // old C++ - we either add this explicit conversion, or remove the
        // overload that handles bool types, as the compiler implicitly
        // chooses that conversion over this one
        PtrToBagObject put (const Text& name, const char* thing) {
            return put (name, Text (thing));
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

        PtrToBagThing getLocal (const Text& name) {
            PtrToBagThing* handle = value.get (name);
            return (handle) ? *handle : (BagThing*)0;
        }

        const PtrToBagThing getLocal (const Text& name) const {
            const PtrToBagThing* handle = value.get (name);
            return (handle) ? *handle : (BagThing*)0;
        }

        virtual const PtrToBagThing get (const Text& path) const {
            vector<Text> parts = path.splitFirst ("/");
            PtrToBagThing ptrToBagThing = getLocal (parts[0]);
            if ((parts.size () == 2) and ptrToBagThing) {
                switch (ptrToBagThing->getType ()) {
                    case BagThing::OBJECT_TYPE:
                    case BagThing::ARRAY_TYPE:
                        return ptr_downcast<BagContainer> (ptrToBagThing)->get (parts[1]);
                    default:
                        return (BagThing*) 0;
                }
            }
            return ptrToBagThing;
        }

        /*
        PtrToBagThing operator [] (const Text& name) {
            return get (name);
        }

        const PtrToBagThing operator [] (const Text& name) const {
            return get (name);
        }
        */
};
