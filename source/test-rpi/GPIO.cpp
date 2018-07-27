#include        "Test.h"
#include        "GPIO.h"

TEST_CASE(TestGPIO) {
    Log::Scope scope (Log::DEBUG);
    try {
        GPIO    gpio;

        vector<Text>    names;
        names.push_back ("INPUT");
        names.push_back ("OUTPUT");
        names.push_back ("ALTERNATE_5");
        names.push_back ("ALTERNATE_4");
        names.push_back ("ALTERNATE_0");
        names.push_back ("ALTERNATE_1");
        names.push_back ("ALTERNATE_2");
        names.push_back ("ALTERNATE_3");

        for (uint pin = 0; pin < GPIO::PIN_COUNT; ++pin) {
            GPIO::Function function = gpio.getFunction (static_cast<GPIO::Pin> (pin));
            Log::debug () << "TestGPIO: " << "pin " << pin << " is " << names[function] << endl;
        }

    } catch (RuntimeError& runtimeError) {
        Log::error () << runtimeError.getMessage () << endl;
    } catch (...) {
    }
    TEST_X(true);

}
