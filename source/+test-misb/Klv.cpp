#include "Test.h"
#include "File.h"
#include "Klv.h"

TEST_MODULE_DEPENDENCIES (Klv, "UniversalLabelKey,MisbByteStreamReader,MisbByteStreamWriter")

TEST_CASE(KlvDictionary) {
    //Log::Scope scope (Log::INFO);

    Text keyText ("06.0e.2b.34.01.01.01.01.0e.01.01.03.2d.02.00.00");
    UniversalLabelKey key (keyText);

    Klv klv (File ("stennis.klv").read());
    PtrToBagObject entry = klv.getKeyDescription(key);
    Text name = entry->getText("Name");
    TEST_EQUALS (name, "Sigma_Height");
}

TEST_CASE(KlvRead) {
    Log::Scope scope (Log::DEBUG);

    Klv klv (File ("stennis.klv").read());
    while (klv.readUniversalSet ()) {}
    TEST_TRUE(true);
}
