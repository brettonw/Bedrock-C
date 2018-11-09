#include "Test.h"
#include "MisbDictionary.h"

TEST_MODULE_DEPENDENCIES (MisbDictionary, "UniversalLabelKey")

TEST_CASE(MisbDictionary) {
    Log::Scope scope (Log::DEBUG);

    Text keyText ("06.0e.2b.34.01.01.01.01.0e.01.01.03.2d.02.00.00");
    UniversalLabelKey key (keyText);

    PtrToMisbDictionary dictionary = MisbDictionary::fromFile ();
    PtrToBagObject entry = dictionary->get(key);
    Text name = entry->getText("Name");
    TEST_EQUALS (name, "Sigma_Height");
}

void addToStandard (PtrToBagObject& standardsObject, const Text& standard, PtrToBagObject& entry) {
    if (not standardsObject->contains(standard)) {
        standardsObject->put (standard, PtrToBagArray (new BagArray ()));
    }
    standardsObject->getBagArray (standard)->add(entry);
}

TEST_CASE(WriteMisbStandard) {
    Log::Scope scope (Log::DEBUG);
    PtrToFile databaseFile = new File (MISB_DATABASE_NAME);
    if (databaseFile->getExists ()) {
        PtrToBagThing database = Json::readFile(databaseFile);
        if (database) {
            PtrToBagArray array = ptr_downcast<BagArray> (database);
            if (array) {
                PtrToBagObject standardsObject (new BagObject ());
                Log::debug() << "Reading MISB database: " << databaseFile->getBasename() << ", with " << array->size() << " entries." << endl;
                for (uint i = 0, end = array->size(); i < end; ++i) {
                    PtrToBagObject entry = array->getBagObject(i);
                    Text standard = entry->contains ("Standard") ? entry->getText("Standard") : "XXX";
                    standard = standard.replace("/", "-");
                    addToStandard (standardsObject, standard, entry);
                }

                // now loop over the keys of the standards object and report the numbers
                vector<Text> keys = standardsObject->keys();
                for (vector<Text>::const_iterator iter = keys.begin (); iter != keys.end (); ++iter) {
                    PtrToBagArray array = standardsObject->getBagArray (*iter);
                    Log::debug() << "  Standard (" << *iter << ") with " << array->size() << " entr" << ((array->size() == 1) ? "y" : "ies") << endl;
                }
            }
        }
    }
    TEST_TRUE (true);
}
