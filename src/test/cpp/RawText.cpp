#include "Test.h"
#include "RawText.h"


TEST_CASE(TestRawText) {
    const char* helloWorld = "Hello World";
    uint helloWorldLength = strlen (helloWorld);
    PtrToRawText rawText = RawText::make (helloWorld, helloWorldLength);
    TESTXY (rawText->getCapacity(), 16);
    TESTXY (rawText->getLength (), helloWorldLength);
    TESTXY (memcmp (helloWorld, rawText->get(), rawText->getLength()), 0);

    const char* hello = "Hello";
    uint helloLength = strlen (hello);
    rawText = RawText::make (helloWorld, helloLength);
    TESTXY (rawText->getCapacity(), 8);
    TESTXY (rawText->getLength (), helloLength);
    TESTXY (memcmp (hello, rawText->get(), rawText->getLength()), 0);

    TESTXY (rawText->append (hello, strlen (hello)), false);
    TESTXY (rawText->getLength (), helloLength);
    TESTXY (memcmp ("Hello", rawText->get(), rawText->getLength()), 0);

    rawText = RawText::make(50);
    rawText->set (hello, helloLength);
    TESTXY (rawText->getCapacity(), 64);
    TESTXY (rawText->getLength (), helloLength);
    TESTXY (memcmp (hello, rawText->get(), rawText->getLength()), 0);
    rawText->append (hello, helloLength);
    TESTXY (rawText->getLength (), helloLength * 2);
    TESTXY (memcmp ("HelloHello", rawText->get(), rawText->getLength()), 0);

    PtrToRawText empty = RawText::getEmpty();
    TESTXY (empty->getCapacity(), 0);
    TESTXY (empty->getLength(), 0);
    TESTXY ((empty->get ())[0], 0);
}
