#pragma once

#include    "BagContainer.h"
#include    "TextContainer.h"

MAKE_PTR_TO_SUB(BagObject, BagContainer) {
    protected:
        TextMap<PtrToBagThing> value;

        PtrToBagObject put (const Text& name, BagThing* bagThing) {
            // look up the requested name in our name table to check to see if we already have a
            // pointer to an identical string - this will mitigate repeated copying of the same
            // name, and reduce the memory footprint of BagObjects in aggregate. we ensure the
            // name string is unique to make it hard to accidentally change the string after using
            // it as a name.
            static TextMap<Text> nameTable;
            const Text* foundName = nameTable.get(name);
            //Log::trace () << "BagObject: " << "name (" << name << ") " << (name.isUnique() ? "is" : "is NOT") << " unique" << endl;
            ///Log::trace () << "           " << "name (" << name << ") " << (foundName ? "is" : "is NOT") << " already in the name table" << endl;
            //Log::trace () << "           " << "nameTable contains " << nameTable.size () << " reference" << ((nameTable.size () != 1 ) ? "s" : "") << endl;
            Text useName = (foundName != 0) ? *foundName : (nameTable[name] = name.makeUnique ());
            //Log::trace () << "           " << "nameTable contains " << nameTable.size () << " reference" << ((nameTable.size () != 1 ) ? "s" : "") << endl;
            //Log::trace () << "           " << "using (" << useName << ") with " << useName.getReferenceCount () << " references" << endl;

            // and now actually assign the thing...
            value[useName] = bagThing;
            return this;
        }

    public:
        BagObject () : BagContainer (BagThing::OBJECT_TYPE) {}
        BagObject (const BagObject& source) : BagContainer (BagThing::OBJECT_TYPE), value (source.value) {}
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
                prepend = ",";
            }
            return enclose (out, "{", "}");
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
