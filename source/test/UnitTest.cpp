#include "UnitTest.h"

UnitTest* UnitTest::currentUnitTest = 0;
BagObject* UnitTest::configurationObject;
TextMap<UnitTest*>* UnitTest::registry;
