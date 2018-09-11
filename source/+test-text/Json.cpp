#include "Test.h"
#include "Json.h"

TEST_CASE(JsonSimple) {
    //Log::Scope scope (Log::TRACE);
    PtrToBagObject bagObject = Json::readBagObject("{ \"xxx\": 1234, \"yyy\": \"abc\" }");
    TEST_EQUALS(bagObject->getText("xxx"), "1234");
    TEST_EQUALS(bagObject->getText("yyy"), "abc");
}

TEST_CASE(JsonArray) {
    Log::Scope scope (Log::TRACE);
    PtrToBagArray bagArray = Json::readBagArray("[ \"xxx\", 1234, \"yyy\", \"abc\" ]");
    TEST_EQUALS(bagArray->getText(1), "1234");
    TEST_EQUALS(bagArray->getText(3), "abc");
}

TEST_CASE(JsonSimpleError) {
    //Log::Scope scope (Log::TRACE);
    PtrToBagObject bagObject = Json::readBagObject("{ \"xxx\": 1234,, \"yyy\": \"abc\" }");
    TEST_FALSE(bagObject);
}
