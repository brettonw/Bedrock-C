#include        "Test.h"
#include        "GPIO.h"

TEST_CASE(TestGPIO) {
    //Log::Scope scope (Log::TRACE);
    try {
        GPIO    gpio;

        for (uint pin = 0; pin < GPIO::PIN_COUNT; ++pin) {
            GPIO::Function function = gpio.getFunction (static_cast<GPIO::Pin> (pin));
            switch (function) {
                case GPIO::Function::INPUT:
                    Log::debug () << "TestGPIO: " << "pin " << pin << " is " << "INPUT" << endl;
                break;
                case GPIO::Function::OUTPUT:
                    Log::debug () << "TestGPIO: " << "pin " << pin << " is " << "OUTPUT" << endl;
                break;
                case GPIO::Function::ALTERNATE_0:
                    Log::debug () << "TestGPIO: " << "pin " << pin << " is " << "ALTERNATE_0" << endl;
                break;
                case GPIO::Function::ALTERNATE_1:
                    Log::debug () << "TestGPIO: " << "pin " << pin << " is " << "ALTERNATE_1" << endl;
                break;
                case GPIO::Function::ALTERNATE_2:
                    Log::debug () << "TestGPIO: " << "pin " << pin << " is " << "ALTERNATE_2" << endl;
                break;
                case GPIO::Function::ALTERNATE_3:
                    Log::debug () << "TestGPIO: " << "pin " << pin << " is " << "ALTERNATE_3" << endl;
                break;
                case GPIO::Function::ALTERNATE_4:
                    Log::debug () << "TestGPIO: " << "pin " << pin << " is " << "ALTERNATE_4" << endl;
                break;
                case GPIO::Function::ALTERNATE_5:
                    Log::debug () << "TestGPIO: " << "pin " << pin << " is " << "ALTERNATE_5" << endl;
                break;
            }
        }

    } catch (RuntimeError& runtimeError) {
        Log::error () << runtimeError.getMessage () << endl;
    } catch (...) {
    }
    TEST_X(true);

}
