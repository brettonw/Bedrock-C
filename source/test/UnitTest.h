#pragma once

//#include "Log.h"
#include "File.h"
#include "Json.h"
#include "Bag.h"

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
        Text name;
        Text module;
        uint assertionsCount;

        static UnitTest* currentUnitTest;
        static BagObject* configurationObject;
        static TextMap<UnitTest*>* registry;

    public:
        UnitTest (const Text& _name, const Text& sourceFileName) : name (_name), module (File (sourceFileName).getBasename ()), assertionsCount (0) {
            if (not registry) {
                registry = new TextMap<UnitTest*> ();
            }
            registry->set(name, this);
        }

        virtual ~UnitTest () {}

        static bool test (bool assertion) {
            if (currentUnitTest) {
                ++currentUnitTest->assertionsCount;
            }
           return assertion;
        }

        virtual void test () = 0;
        virtual Text getDependencies () const = 0;

        static void runOne (const Text& testName) {
            // ignore any test dependencies
            UnitTest** test = registry->get(testName);
            if (test) {
                (*test)->callTest ();
            }
        }

        static void runAll () {

        }

        void callTest () {
            try {
                currentUnitTest = this;
                int logLevel = getLogLevel ();
                if (logLevel >= 0) {
                    Log::info () << "TEST: (" << module << ") " << name << endl;
                    {
                        Log::Scope scope (static_cast<uint> (logLevel));
                        test ();
                    }
                    if (assertionsCount > 0) {
                        Log::debug () << "PASSED: " << name << endl << endl;
                    } else {
                        Log::error () << "EMPTY: " << name << " (made no assertions)" << endl << endl;
                        exit (EXIT_FAILURE);
                    }
                } else {
                    Log::info () << "SKIP: (" << module << ") " << name << endl;
                }
            } catch (RuntimeError& runtimeError) {
                Log::exception (runtimeError);
                exit (EXIT_FAILURE);
            } catch (exception& exception) {
                cerr << "[OTHER] " << exception.what () << endl;
                exit (EXIT_FAILURE);
            } catch (...) {
                cerr << "[ UNKN] exiting..." << endl;
                exit (EXIT_FAILURE);
            }
        }

        int getLogLevel () {
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
                if (not moduleObject->contains (name)) {
                    moduleObject->put (name, "WARN");
                }
                logLevel = logLevelFromText (moduleObject->getText (name));
            }

            return logLevel;
        }

        static void writeConfiguration () {
            Text json = configurationObject->toJson() << "\n";
            File configurationFile ("unit-test-configuration.json");
            configurationFile.writeText(json);
        }
};
