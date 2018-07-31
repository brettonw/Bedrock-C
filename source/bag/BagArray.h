#pragma once

#include    "BagContainer.h"

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

        struct SimpleSort {
                bool operator () (PtrToBagThing left,PtrToBagThing right) {
                    // should left be ordered before right?
                    return false;
                }
        };

        PtrToBagArray   sort () {
            return this;
        }
};
