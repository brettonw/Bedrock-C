#include "Test.h"
#include "Json.h"

TEST_CASE(JsonSimple) {
    Log::Scope scope (Log::TRACE);
    PtrToBagObject bagObject = Json::readBagObject("{ \"xxx\": 1234, \"yyy\": \"abc\" }");
    TEST_EQUALS(bagObject->getText("xxx"), "1234");
    TEST_EQUALS(bagObject->getText("yyy"), "abc");
}

TEST_CASE(JsonSimpleError) {
    Log::Scope scope (Log::TRACE);
    PtrToBagObject bagObject = Json::readBagObject("{ \"xxx\": 1234,, \"yyy\": \"abc\" }");
    TEST_TRUE(true);
}
