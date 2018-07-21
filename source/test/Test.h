#include    "Log.h"

#pragma once

class UnitTest {
    protected:
        uint assertionsCount;

    public:
        UnitTest () : assertionsCount (0) { currentUnitTest = this; }
        virtual ~UnitTest () {}

        static bool test (bool assertion) {
            if (currentUnitTest) {
                ++currentUnitTest->assertionsCount;
            }
           return assertion;
        }

        static UnitTest* currentUnitTest;
};

#define     TEST_ASSERTION(x) if (not (UnitTest::test (x))) { char* buffer = new char[128]; sprintf (buffer, "ASSERTION FAILURE (FILE: %s, LINE: %d)", __FILE__, __LINE__); throw runtime_error (buffer); }
#define     TEST_XYOP(x, y, op) { bool result_xyop = ((x) op (y)); (result_xyop ? (Log::debug () << "    PASS") : (Log::error () << "    FAIL")) << ": ((" #x ") " #op " (" #y ")) (" << (x) << " " #op " " << (y) << ")" << endl; TEST_ASSERTION(result_xyop); }
#define     TEST_XYF(x, y, p) TEST_XYOP(fabs (x - y), p, <=)
#define     TEST_XY(x, y) TEST_XYOP(x, y, ==)
#define     TEST_X(x) TEST_XYOP(x, true, ==)
#define		TEST_CASE(name)										                                            \
			class XXX##name : public UnitTest {  				                                            \
				public:											                                            \
					XXX##name () {			                                                                \
						try {									                                            \
						    Log::info () << "TESTING: (" __FILE__ << ") " << #name << endl;                 \
							test ();				                                                        \
							if (assertionsCount > 0) {                                                      \
							    Log::debug () << "PASSED: " #name << endl << endl;	                        \
							} else {                                                                        \
                                Log::error () << "EMPTY: " #name " (made no assertions)" << endl << endl;   \
                                exit (EXIT_FAILURE);                                                        \
							}                                                                               \
						} catch (exception& exception) {		                                            \
							cerr << "    " << exception.what () << endl;                                    \
							exit (EXIT_FAILURE);                                                            \
						}										                                            \
					}											                                            \
					void test ();								                                            \
        			static XXX##name test##name;			                                                \
			};													                                            \
        	XXX##name XXX##name::test##name;			                                                    \
			inline void XXX##name::test ()
