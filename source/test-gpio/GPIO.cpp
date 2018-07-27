#include        "Test.h"
#include        "GPIO.h"

TEST_CASE(TestGPIO) {
    Log::Scope scope (Log::DEBUG);
    try {
        GPIO    gpio;

        vector<Text>    names;
        names.push_back ("INPUT ");
        names.push_back ("OUTPUT");
        names.push_back ("ALT_5 ");
        names.push_back ("ALT_4 ");
        names.push_back ("ALT_0 ");
        names.push_back ("ALT_1 ");
        names.push_back ("ALT_2 ");
        names.push_back ("ALT_3 ");

        for (uint pin = 0; pin < GPIO_PIN_COUNT;) {
            GPIO::Function function = gpio.getFunction (static_cast<Pin> (pin));
            //Log::debug () << "TestGPIO: " << "pin " << pin << " is " << names[function] << endl;
            Log& log = Log::debug () << "TestGPIO: " << pin << " -> " << names[function] << " | ";
            ++pin;
            function = gpio.getFunction (static_cast<Pin> (pin + 1));
            log << names[function] << " <- " << pin << endl;
            ++pin;
        }

    } catch (RuntimeError& runtimeError) {
        Log::exception (runtimeError);
    } catch (...) {
    }
    TEST_X(true);
}
