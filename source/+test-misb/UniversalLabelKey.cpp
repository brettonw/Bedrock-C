#include "Test.h"
#include "Klv.h"

TEST_CASE(UniversalLabelKey) {
    Text key ("06.0e.2b.34.01.01.01.01.0e.01.01.03.2d.02.00.00");
    UniversalLabelKey ulk (key);
    Text keyText = ulk.toText();
    TEST_EQUALS (key, keyText);
}

TEST_CASE(Decode) {
    // UAS Local Data Set
    UniversalLabelKey ulk ("06.0e.2b.34.02.0b.01.01.0e.01.03.01.01.00.00.00");
    TEST_EQUALS (ulk.getCategoryDesignator(), UniversalLabelKey::CATEGORY_GROUPS);
    TEST_EQUALS (ulk.getRegistryDesignator(), 0x0b);
    TEST_EQUALS (ulk.getRegistryGroupType(), UniversalLabelKey::LOCAL_DATA_SET);
    TEST_EQUALS (ulk.getRegistryLengthType(), UniversalLabelKey::LENGTH_TYPE_BER);
    TEST_EQUALS (ulk.getRegistrar(), UniversalLabelKey::REGISTRAR_MISB);
    TEST_EQUALS (ulk.getDataType(), UniversalLabelKey::TYPE_NOTHING);

    // Universal Timestamp
    ulk = UniversalLabelKey ("06.0E.2B.34.01.01.01.03.07.02.01.01.01.05.00.00");
    TEST_EQUALS (ulk.getCategoryDesignator(), UniversalLabelKey::CATEGORY_DATA);
    TEST_EQUALS (ulk.getRegistryLengthType(), UniversalLabelKey::LENGTH_TYPE_BER);
    TEST_EQUALS (ulk.getRegistrar(), UniversalLabelKey::REGISTRAR_SMPTE);
    TEST_EQUALS (ulk.getDataType(), UniversalLabelKey::TYPE_METADATA);

    // URL, UTF16
    ulk = UniversalLabelKey ("06.0E.2B.34.01.01.01.01.01.02.01.01.01.00.00.00");
    TEST_EQUALS (ulk.getCategoryDesignator(), UniversalLabelKey::CATEGORY_DATA);
    TEST_EQUALS (ulk.getRegistryLengthType(), UniversalLabelKey::LENGTH_TYPE_BER);
    TEST_EQUALS (ulk.getRegistrar(), UniversalLabelKey::REGISTRAR_SMPTE);
    TEST_EQUALS (ulk.getDataType(), UniversalLabelKey::TYPE_METADATA);
    TEST_EQUALS (ulk.getDataRepresentationVariant(), 0x01);
}
