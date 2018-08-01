#pragma once

#include    "BagContainer.h"

// sort helpers
enum SortDir { DESCENDING, ASCENDING };

class SimpleSort {
    private:
        SortDir sortDir;

    public:
        SimpleSort (SortDir _sortDir) : sortDir (_sortDir) {}

        double compare (const PtrToBagThing& leftThing,const PtrToBagThing& rightThing) const {
            // strategy:
            // start by trying to do a numeric comparison, if that's not possible, convert
            // both sides to strings and do a string comparison
            double delta = leftThing->sortValue() - rightThing->sortValue();
            if (delta == 0) {
                Log::debug() << "SimpleSort: " << "comparing as text ([" << leftThing->toText() << "], [" <<  rightThing->toText () << "])" << endl;
                delta = static_cast<double> (leftThing->toText().compare(rightThing->toText ()));
            }
            return (sortDir == ASCENDING) ? delta : -delta;
        }
};

class FieldSort {
    private:
        Text field;
        SimpleSort simpleSort;

    public:
        FieldSort (const Text& _field, SortDir sortDir) : field (_field), simpleSort (sortDir) {}

        double compare (const PtrToBagObject& leftObject, const PtrToBagObject& rightObject) const {
            return simpleSort.compare (leftObject->get(field), rightObject->get(field));
        }

        double compare (const PtrToBagThing& leftThing,const PtrToBagThing& rightThing) const {
            return compare (ptr_downcast<BagObject> (leftThing), ptr_downcast<BagObject> (rightThing));
        }
};

class DbSort {
    private:
        vector<FieldSort> fields;

    public:
        DbSort (const Text& field, SortDir sortDir = ASCENDING) {
            fields.push_back (FieldSort (field, sortDir));
        }

        DbSort& operator () (const Text& field, SortDir sortDir = ASCENDING) {
            fields.push_back (FieldSort (field, sortDir));
            return *this;
        }

        double compare (const PtrToBagObject& leftObject, const PtrToBagObject& rightObject) const {
            for (vector<FieldSort>::const_iterator i = fields.begin (); i != fields.end (); ++i) {
                double result = i->compare (leftObject, rightObject);
                if (result != 0) {
                    return result;
                }
            }
            return 0;
        }

        double compare (const PtrToBagThing& leftThing,const PtrToBagThing& rightThing) const {
            return compare (ptr_downcast<BagObject> (leftThing), ptr_downcast<BagObject> (rightThing));
        }
};

// the actual bag array
MAKE_PTR_TO_SUB(BagArray, BagContainer) {
    protected:
        vector<PtrToBagThing> value;
        friend class BagArraySort;

        PtrToBagArray add (BagThing* bagThing) {
            value.push_back (bagThing);
            return this;
        }

        template<typename SortType>
        struct SortAdapter {
            const SortType& sortType;

            SortAdapter (const SortType& _sortType) : sortType (_sortType) {}

            // should left be ordered before right?
            bool operator () (const PtrToBagThing& leftThing,const PtrToBagThing& rightThing) {
                return sortType.compare (leftThing, rightThing) < 0;
            }
        };

        template<typename SortType>
        static PtrToBagArray withSortAdapter (const PtrToBagArray& source, const SortType& sortType) {
            SortAdapter<SortType> sortAdapter (sortType);
            PtrToBagArray copy = new BagArray (*source);
            ::sort (copy->value.begin(), copy->value.end(), sortAdapter);
            return copy;
        }

    public:
        BagArray () : BagContainer (BagThing::ARRAY_TYPE) {}
        BagArray (const BagArray& source) : BagContainer (BagThing::ARRAY_TYPE), value (source.value) {}

        virtual ~BagArray () {}

        virtual Text toJson () const {
            Text prepend = "";
            Text out;
            for (vector<PtrToBagThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << (*it)->toJson ();
                prepend = ",";
            }
            return enclose (out, "[", "]");
        }

        virtual Text toText () const {
            const char* prepend = "";
            Text out;
            for (vector<PtrToBagThing>::const_iterator it = value.begin (); it != value.end (); ++it) {
                out << prepend << (*it)->toText ();
                prepend = ", ";
            }
            return out;
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

        static PtrToBagArray sort (const PtrToBagArray& source, SortDir sortDir = ASCENDING) {
            Log::debug() << "BagArray: " << "simple sort" << endl;
            return withSortAdapter (source, SimpleSort (sortDir));
        }

        static PtrToBagArray sort (const PtrToBagArray& source, const Text& field, SortDir sortDir = ASCENDING) {
            Log::debug() << "BagArray: " << "field sort" << endl;
            return withSortAdapter (source, FieldSort (field, sortDir));
        }

        static PtrToBagArray sort (const PtrToBagArray& source, const DbSort& dbSort) {
            Log::debug() << "BagArray: " << "db sort" << endl;
            return withSortAdapter (source, dbSort);
        }
};
