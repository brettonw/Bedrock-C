#pragma once

#include    "BagArray.h"
#include    "BagObject.h"

class BagArraySort {
    private:
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
        static PtrToBagArray sort (const PtrToBagArray& source, SortDir sortDir = ASCENDING) {
            Log::debug() << "BagArraySort: " << "simple sort" << endl;
            return withSortAdapter (source, SimpleSort (sortDir));
        }

        static PtrToBagArray sort (const PtrToBagArray& source, const Text& field, SortDir sortDir = ASCENDING) {
            Log::debug() << "BagArraySort: " << "field sort" << endl;
            return withSortAdapter (source, FieldSort (field, sortDir));
        }

        static PtrToBagArray sort (const PtrToBagArray& source, const DbSort& dbSort) {
            Log::debug() << "BagArraySort: " << "db sort" << endl;
            return withSortAdapter (source, dbSort);
        }
};
