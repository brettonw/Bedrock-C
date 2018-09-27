#include "Test.h"
#include "File.h"
#include "Klv.h"

TEST_MODULE_DEPENDENCIES (Klv, "UniversalLabelKey,MisbByteStreamReader,MisbByteStreamWriter")

TEST_CASE(KlvDictionary) {
    Text keyText ("06.0e.2b.34.01.01.01.01.0e.01.01.03.2d.02.00.00");
    UniversalLabelKey key (keyText);

    Klv klv ("dictionary.json", File ("stennis.klv").read());
    PtrToBagObject entry = klv.getKeyDescription(key);
    Text symbol = entry->getText("Symbol");
    TEST_EQUALS (symbol, "sigma_height");
}


TEST_CASE(KlvBuffer) {
    Log::Scope scope (Log::DEBUG);

    Klv klv ("dictionary.json", File ("stennis.klv").read());
    klv.readUniversalDataSet ();
    TEST_TRUE(true);
}
