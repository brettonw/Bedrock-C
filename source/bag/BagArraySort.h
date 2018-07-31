#pragma once

#include    "BagArray.h"
#include    "BagObject.h"

class BagArraySort {
    private:
    class SimpleSort {
        private:
            bool ascending;

        public:
            SimpleSort (bool _ascending) : ascending (_ascending) {}

            double compare (const PtrToBagThing& leftThing,const PtrToBagThing& rightThing) const {
                // strategy:
                // start by trying to do a numeric comparison, if that's not possible, convert
                // both sides to strings and do a string comparison
                double delta = leftThing->sortValue() - rightThing->sortValue();
                if (delta == 0) {
                    delta = static_cast<double> (leftThing->toText().compare(rightThing->toText ()));
                }
                return ascending ? delta : -delta;
            }
    };

    class FieldSort {
        private:
            Text field;
            SimpleSort simpleSort;

        public:
            FieldSort (const Text& _field, bool ascending = true) : field (_field), simpleSort (ascending) {}

            double compare (const PtrToBagObject& leftObject, const PtrToBagObject& rightObject) const {
                return simpleSort.compare (leftObject->get(field), rightObject->get(field));
            }

            double compare (const PtrToBagThing& leftThing,const PtrToBagThing& rightThing) const {
                return compare (ptr_downcast<BagObject> (leftThing), ptr_downcast<BagObject> (rightThing));
            }
    };

    template<typename SortType>
    struct SortAdapter {
        const SortType& sortType;

        SortAdapter (const SortType& _sortType) : sortType (_sortType) {}

        // should left be ordered before right?
        bool operator () (const PtrToBagThing& leftThing,const PtrToBagThing& rightThing) {
            return sortType.compare (leftThing, rightThing) < 0;
        }
    };

    public:

        class DbSort {
            private:
                vector<FieldSort> fields;

            public:
                DbSort (const Text& field, bool ascending) {
                    fields.push_back (FieldSort (field, ascending));
                }

                DbSort& operator () (const Text& field, bool ascending = true) {
                    fields.push_back (FieldSort (field, ascending));
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

        static PtrToBagArray sort (const PtrToBagArray& source, bool ascending = true) {
            SimpleSort simpleSort (ascending);
            SortAdapter<SimpleSort> sortAdapter (simpleSort);
            PtrToBagArray copy = new BagArray (*source);
            ::sort (copy->value.begin(), copy->value.end(), sortAdapter);
            return copy;
        }

        static PtrToBagArray sort (const PtrToBagArray& source, const Text& field, bool ascending = true) {
            FieldSort fieldSort (field, ascending);
            SortAdapter<FieldSort> sortAdapter (fieldSort);
            PtrToBagArray copy = new BagArray (*source);
            ::sort (copy->value.begin(), copy->value.end(), sortAdapter);
            return copy;
        }

        static PtrToBagArray sort (const PtrToBagArray& source, const DbSort& dbSort) {
            SortAdapter<DbSort> sortAdapter (dbSort);
            PtrToBagArray copy = new BagArray (*source);
            ::sort (copy->value.begin(), copy->value.end(), sortAdapter);
            return copy;
        }

};
