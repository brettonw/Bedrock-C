#include "Test.h"
#include "File.h"
#include "Klv.h"

TEST_MODULE_DEPENDENCIES (Klv, "UniversalLabelKey,MisbByteStreamReader,MisbByteStreamWriter")

TEST_CASE(KlvRead) {
    //Log::Scope scope (Log::DEBUG);

    File klvDirectory ("klv");
    vector<PtrToFile> files = klvDirectory.getFiles();
    for (vector<PtrToFile>::iterator iter = files.begin (); iter != files.end (); ++iter) {
        if ((*iter)->getBasename() != "xxx") {
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
    TEST_TRUE(true);
}

TEST_CASE(ST0902) {
    Log::Scope scope (Log::DEBUG);

    // this test file contains two UAS datalink packets given in ST0902.7, section 9, Annex C
    // the standard contains an omission in data for tag 0x14 where it sets the bytes to the value
    // 00 00 00 00. The checksum given is correct for this case, but the example shows a different
    // value of 7D C5 5E CE. When changed in the packet, this causes the checksum to be incorrect.
    // we corrected the checksums in our data file.
    File st0902 ("klv/0902.hex");
    Text hex0902 = st0902.readText();
    vector<Text> hexBytes = hex0902.split (" ");
    vector<byte> bytes;
    for (vector<Text>::iterator iter = hexBytes.begin (); iter != hexBytes.end (); ++iter) {
        bytes.push_back(fromHex (*iter));
    }
    Klv (Buffer::make (bytes.data(), bytes.size())).readUniversalSet ();
    TEST_TRUE(true);
}
