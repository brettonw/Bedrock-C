#include        "Test.h"
#include        "Pause.h"
#include        "GPIO.h"

TEST_CASE(TestGPIO) {
    Log::Scope scope (Log::DEBUG);
    try {
        GPIO    gpio;

        const char* functions[] = {
            "I", "O", "F", "E", "A", "B", "C", "D"
        };

        GPIO::Function function;
        Log& log = Log::debug () << "TestGPIO: pinout" << endl << endl;
        for (uint piPin = 0; piPin < RASPBERRY_PI_PIN_COUNT;) {
            log << ((piPin < 10) ? " " : "");
            try {
                function = gpio.getFunction (static_cast<Pin> (getPin (static_cast<PiPin> (piPin))));
                log << (piPin + 1) << " -> " << functions[function] << " | ";
            } catch (RuntimeError& runtimeError) {
                log << (piPin + 1) << " -> " << " * " << " | ";
            }
            ++piPin;
            try {
                function = gpio.getFunction (static_cast<Pin> (getPin (static_cast<PiPin> (piPin))));
                log << functions[function] << " <- " << (piPin + 1) << endl;
            } catch (RuntimeError& runtimeError) {
                log << " * " << " <- " << (piPin + 1) << endl;
            }
            ++piPin;
        }
        log << endl;

    } catch (RuntimeError& runtimeError) {
        Log::exception (runtimeError);
    } catch (...) {
    }
    TEST_X(true);
}

TEST_CASE(TestBlinkPin6) {
    Log::Scope scope (Log::DEBUG);
    try {
        GPIO    gpio;
        for (int i = 0; i < 10; ++i) {
            gpio.set (GPIO_06);
            Pause::milli (500);
            gpio.clear (GPIO_06);
            Pause::milli (500);
        }
    } catch (RuntimeError& runtimeError) {
        Log::exception (runtimeError);
    } catch (...) {
    }
    TEST_X(true);
}
