#include "Test.h"
#include "File.h"
#include "Klv.h"

TEST_MODULE_DEPENDENCIES (Klv, "UniversalLabelKey,MisbByteStreamReader,MisbByteStreamWriter")

TEST_CASE(KlvRead) {
    Log::Scope scope (Log::DEBUG);

    File klvDirectory ("klv");
    vector<PtrToFile> files = klvDirectory.getFiles();
    for (vector<PtrToFile>::iterator iter = files.begin (); iter != files.end (); ++iter) {
        if ((*iter)->getBasename() == "stennis") {
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
    }

    // drop an accumulated version of the dictionary
    //Klv::makeDictionary();

    TEST_TRUE(true);
}
