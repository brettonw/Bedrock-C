#include "Test.h"
#include "Klv.h"


TEST_CASE(UniversalLabelKey) {
    Text key ("06.0e.2b.34.01.01.01.01.0e.01.01.03.2d.02.00.00");
    UniversalLabelKey ulk (key);
    Text keyText = ulk.toText();
    TEST_EQUALS (key, keyText);
}
