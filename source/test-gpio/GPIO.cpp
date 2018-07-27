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

        GPIO::Function function;
        for (uint piPin = 0; piPin < RASPBERRY_PI_PIN_COUNT;) {
            function = gpio.getFunction (static_cast<Pin> (getPin (static_cast<PiPin> (piPin))));
            Log& log = Log::debug () << "TestGPIO: " << piPin << " -> " << names[function] << " | ";
            ++piPin;
            function = gpio.getFunction (static_cast<Pin> (getPin (static_cast<PiPin> (piPin))));
            log << names[function] << " <- " << piPin << endl;
            ++piPin;
        }

    } catch (RuntimeError& runtimeError) {
        Log::exception (runtimeError);
    } catch (...) {
    }
    TEST_X(true);
}
