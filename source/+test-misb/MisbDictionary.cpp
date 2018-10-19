#include "Test.h"
#include "MisbDictionary.h"

TEST_MODULE_DEPENDENCIES (MisbDictionary, "UniversalLabelKey")

TEST_CASE(MisbDictionary) {
    Log::Scope scope (Log::DEBUG);

    Text keyText ("06.0e.2b.34.01.01.01.01.0e.01.01.03.2d.02.00.00");
    UniversalLabelKey key (keyText);

    MisbDictionary dictionary (MISB_DIR "misb-dictionary.json");
    PtrToBagObject entry = dictionary.get(key);
    Text name = entry->getText("Name");
    TEST_EQUALS (name, "Sigma_Height");
}

/*
TEST_CASE(Rewrite) {
    MisbDictionary dictionary (MISB_DIR "misb-dictionary.json");
    dictionary.rewrite(MISB_DIR "misb-database.json");
}
*/
