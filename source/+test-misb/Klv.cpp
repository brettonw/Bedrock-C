#include "Test.h"
#include "File.h"
#include "Klv.h"

TEST_MODULE_DEPENDENCIES (Klv, "UniversalLabelKey")

TEST_CASE(KlvDictionary) {
    Text keyText ("06.0e.2b.34.01.01.01.01.0e.01.01.03.2d.02.00.00");
    UniversalLabelKey key (keyText);

    Klv klv ("dictionary.json");
    PtrToBagObject entry = klv.getKeyDescription(key);
    Text symbol = entry->getText("Symbol");
    TEST_EQUALS (symbol, "sigma_height");
}


TEST_CASE(KlvBuffer) {
    PtrToBuffer buffer = File ("stennis.klv").read();
    TEST_TRUE(true);
}
