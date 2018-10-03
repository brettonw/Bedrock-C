#include "Test.h"
#include "Klv.h"

TEST_CASE(UniversalLabelKey) {
    Text key ("06.0e.2b.34.01.01.01.01.0e.01.01.03.2d.02.00.00");
    UniversalLabelKey ulk (key);
    Text keyText = ulk.toText();
    TEST_EQUALS (key, keyText);
}

TEST_CASE(Decode) {
    Text key ("06.0e.2b.34.02.0b.01.01.0e.01.03.01.01.00.00.00");
    UniversalLabelKey ulk (key);
    TEST_EQUALS (ulk.getCategoryDesignator(), UniversalLabelKey::CATEGORY_GROUPS);
    TEST_EQUALS (ulk.getRegistryDesignator(), 0x0b);
    TEST_EQUALS (ulk.getRegistryGroupType(), UniversalLabelKey::LOCAL_DATA_SET);
    TEST_EQUALS (ulk.getRegistryLengthType(), UniversalLabelKey::TYPE_1_BYTE);
}
