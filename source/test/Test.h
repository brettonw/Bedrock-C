#include "Log.h"
#include "File.h"
#include "Json.h"
#include "Bag.h"

#pragma once

class UnitTest {
    private:
        static int logLevelFromText (const Text& configuration) {
            if (configuration) {
                if (configuration == "TRACE") {
                    return static_cast<int> (Log::TRACE);
                }
                if (configuration == "DEBUG") {
                    return static_cast<int> (Log::DEBUG);
                }
                if (configuration == "INFO") {
                    return static_cast<int> (Log::INFO);
                }
                if (configuration == "WARN") {
                    return static_cast<int> (Log::WARN);
                }
                if (configuration == "ERROR") {
                    return static_cast<int> (Log::ERROR);
                }
                if (configuration == "SKIP") {
                    return -1;
                }
            }
            return static_cast<int> (Log::WARN);
        }

    protected:
        uint assertionsCount;

        static UnitTest* currentUnitTest;
        static BagObject* configurationObject;

    public:
        UnitTest () : assertionsCount (0) { currentUnitTest = this; }
        virtual ~UnitTest () {}

        static bool test (bool assertion) {
            if (currentUnitTest) {
                ++currentUnitTest->assertionsCount;
            }
           return assertion;
        }

        int getLogLevel (const Text& module, const Text& test) {
            int logLevel = static_cast<int> (Log::WARN);
            if (not configurationObject) {
                PtrToBagObject ptrToConfigurationObject;

                // try to read from a configuration file
                File configurationFile ("unit-test-configuration.json");
                if (configurationFile.getExists ()) {
                    Text configurationText = configurationFile.readText();
                    ptrToConfigurationObject = Json::readBagObject(configurationText).getPtr();
                }

                if (not ptrToConfigurationObject) {
                    ptrToConfigurationObject = new BagObject ();
                }

                // orphan this object so it doesn't get deleted when this scope exits
                ptrToConfigurationObject->addRef();
                configurationObject = ptrToConfigurationObject.getPtr();
            }

            if (configurationObject) {
                // get the module object, creating it if necessary
                if (not configurationObject->contains (module)) {
                    configurationObject->put (module, PtrToBagObject (new BagObject ()));
                }
                PtrToBagObject moduleObject = configurationObject->getBagObject(module);

                // get the test log level, creating it if necessary
                if (not moduleObject->contains (test)) {
                    moduleObject->put (test, "WARN");
                }
                logLevel = logLevelFromText (moduleObject->getText (test));
            }

            return logLevel;
        }

        static void writeConfiguration () {
            Text json = configurationObject->toJson() << "\n";
            File configurationFile ("unit-test-configuration.json");
            configurationFile.writeText(json);
        }
};

#define     TEST_ASSERTION(_x) if (not (UnitTest::test (_x))) { char* buffer = new char[128]; sprintf (buffer, "ASSERTION FAILURE (FILE: %s, LINE: %d)", __FILE__, __LINE__); throw RuntimeError (buffer); }
#define     TEST_XYOP(_x, _y, _op) { bool result_xyop = ((_x) _op (_y)); (result_xyop ? (Log::debug () << "    PASS") : (Log::error () << "    FAIL")) << ": ((" #_x ") " #_op " (" #_y ")), (" << (_x) << " " #_op " " << (_y) << ")" << endl; TEST_ASSERTION(result_xyop); }
#define     TEST_EQUALS_ULP(_x, _y) { bool result_xyop = ulpEquals((_x), (_y)); (result_xyop ? (Log::debug () << "    PASS") : (Log::error () << "    FAIL")) << ": ((" #_x ") ~= (" #_y ")), (" << (_x) << " ~= " << (_y) << "), (delta = " << abs ((_x) - (_y)) << ")" << endl; TEST_ASSERTION(result_xyop); }
#define     TEST_IN_RANGE(_x, _min, _max) { bool result_xyop = (_min <= (_x)) and ((_x) <= _max); (result_xyop ? (Log::debug () << "    PASS") : (Log::error () << "    FAIL")) << ": ((" #_min ") <= (" #_x ") <= (" #_max ")), ((" << (_min) << " <= " << (_x) << " <= (" << (_max) << ")" << endl; TEST_ASSERTION(result_xyop); }
#define     TEST_EQUALS_EPS(_x, _y, _e) { bool result_xyop = abs((_x) - (_y)) <= _e; (result_xyop ? (Log::debug () << "    PASS") : (Log::error () << "    FAIL")) << ": ((" #_x ") ~= (" #_y ")), (" << (_x) << " ~= " << (_y) << "), (delta: " << abs ((_x) - (_y)) << " < eps: " << (_e) << ")" << endl; TEST_ASSERTION(result_xyop); }
#define     TEST_EQUALS(_x, _y) TEST_XYOP(_x, _y, ==)
#define     TEST_NOT_EQUALS(_x, _y) TEST_XYOP(_x, _y, !=)
#define     TEST_TRUE(_x) TEST_XYOP(bool (_x), true, ==)
#define     TEST_FALSE(_x) TEST_XYOP(bool (_x), false, ==)
#define     TEST_NYI Log::debug () << "Not Yet Implemented" << endl; UnitTest::test (true)
#define     EXPECT_FAIL(_x)                                                                                     \
            {                                                                                                   \
                try {                                                                                           \
                    _x;                                                                                         \
                    TEST_FALSE(#_x " succeeded (UNEXPECTED)");                                                  \
                } catch (RuntimeError& runtimeError) {                                                          \
                    TEST_TRUE(#_x " failed (EXPECTED)");                                                        \
                    Log::exception (runtimeError);                                                              \
                }                                                                                               \
            }
#define		TEST_CASE(name)										                                                \
			class TEST_CASE_##name : public UnitTest {  				                                        \
				public:											                                                \
					TEST_CASE_##name () {			                                                            \
						try {									                                                \
						    Text module (File (__FILE__).getBasename ());                                       \
						    int logLevel = getLogLevel (module, #name);                                         \
						    if (logLevel >= 0) {                                                                \
                                Log::info () << "TEST: (" << module << ") " << #name << endl;                \
                                {                                                                               \
                                    Log::Scope scope (static_cast<uint> (logLevel));                            \
                                    test ();				                                                    \
                                }                                                                               \
                                if (assertionsCount > 0) {                                                      \
                                    Log::debug () << "PASSED: " #name << endl << endl;	                        \
                                } else {                                                                        \
                                    Log::error () << "EMPTY: " #name " (made no assertions)" << endl << endl;   \
                                    exit (EXIT_FAILURE);                                                        \
                                }                                                                               \
						    } else {                                                                            \
                                Log::info () << "SKIP: (" << module << ") " << #name << endl;               \
						    }                                                                                   \
                        } catch (RuntimeError& runtimeError) {                                                  \
                            Log::exception (runtimeError);                                                      \
                            exit (EXIT_FAILURE);                                                                \
                        } catch (exception& exception) {                                                        \
                            cerr << "[OTHER] " << exception.what () << endl;                                    \
                            exit (EXIT_FAILURE);                                                                \
                        } catch (...) {                                                                         \
                            cerr << "[ UNKN] exiting..." << endl;                                               \
                            exit (EXIT_FAILURE);                                                                \
                        }                                                                                       \
					}											                                                \
					void test ();								                                                \
        			static TEST_CASE_##name test##name;			                                                \
			};													                                                \
        	TEST_CASE_##name TEST_CASE_##name::test##name;			                                            \
			inline void TEST_CASE_##name::test ()
