#include "Test.h"
#include "File.h"
#include "Klv.h"

TEST_MODULE_DEPENDENCIES (Klv, "UniversalLabelKey,MisbByteStreamReader,MisbByteStreamWriter")

TEST_CASE(KlvDictionary) {
    //Log::Scope scope (Log::INFO);

    Text keyText ("06.0e.2b.34.01.01.01.01.0e.01.01.03.2d.02.00.00");
    UniversalLabelKey key (keyText);

    Klv klv (Buffer::make (0));
    PtrToBagObject entry = klv.getKeyDescription(key);
    Text name = entry->getText("Name");
    TEST_EQUALS (name, "Sigma_Height");
}

TEST_CASE(KlvRead) {
    Log::Scope scope (Log::DEBUG);
    File klvDirectory ("klv");
    vector<PtrToFile> files = klvDirectory.getFiles();
    for (vector<PtrToFile>::iterator iter = files.begin (); iter != files.end (); ++iter) {
        Log::debug () << "Reading " << (*iter)->getBasename() << endl;
        if ((*iter)->getExtension() == "klv") {
            Klv klv ((*iter)->read());
            //try {
                klv.readUniversalSet ();
                //break;
            //} catch (RuntimeError& error) {
            //    Log::exception(error);
            //}
        }
    }

    // drop an accumulated version of the dictionary
    PtrToBagObject misbDictionary = Klv::makeDictionary();
    File output ("misb-dictionary.json");
    output.writeText(misbDictionary->toJson());
    TEST_TRUE(true);
}
