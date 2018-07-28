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
        Log& log = Log::debug () << "TestGPIO: " << "Raspberry Pi pinout" << endl << endl;
        for (uint piPin = 0; piPin < RASPBERRY_PI_PIN_COUNT;) {
            log << ((piPin < 10) ? " " : "");
            try {
                Pin pin = getPin (static_cast<PiPin> (piPin));
                function = gpio.getFunction (pin);
                log << (piPin + 1) << " -> " << functions[function] << " | ";
            } catch (RuntimeError& runtimeError) {
                log << (piPin + 1) << " -> " << "*" << " | ";
            }
            ++piPin;
            try {
                Pin pin = getPin (static_cast<PiPin> (piPin));
                function = gpio.getFunction (pin);
                log << functions[function] << " <- " << (piPin + 1) << endl;
            } catch (RuntimeError& runtimeError) {
                log << "*" << " <- " << (piPin + 1) << endl;
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
        gpio.setFunction (GPIO_06, GPIO::Function::OUTPUT);
        TEST_XY(gpio.getFunction (GPIO_06), GPIO::Function::OUTPUT);
        gpio.clear (GPIO_06);
        TEST_XY(gpio.get (GPIO_06), false);

        for (int i = 0; i < 10; ++i) {
            Log::debug () << "TestBlinkPin6: ON" << endl;
            gpio.toggle (GPIO_06);
            Pause::milli (500);
            Log::debug () << "TestBlinkPin6: OFF" << endl;
            gpio.toggle (GPIO_06);
            Pause::milli (500);
        }
    } catch (RuntimeError& runtimeError) {
        Log::exception (runtimeError);
    } catch (...) {
    }
    TEST_X(true);
}

TEST_CASE(TestPinMappings) {
    Log::Scope scope (Log::DEBUG);
    try {
        // this should throw an exception
        getPin (RPI_01);
        TEST_X (false);
    } catch (RuntimeError& runtimeError) {
        TEST_X (true);
    }
    TEST_XY(getPin (RPI_31), GPIO_06);
    TEST_XY(getPin (RPI_40), GPIO_21);

    TEST_XY(getPiPin (GPIO_00), RPI_27);
    TEST_XY(getPiPin (GPIO_06), RPI_31);
    TEST_XY(getPiPin (GPIO_21), RPI_40);
}
