#include    "Types.h"

#pragma once

#define     TEST_ASSERTION(x) if (not (x)) { char* buffer = new char[128]; sprintf (buffer, "ASSERTION FAILURE (FILE: %s, LINE: %d)", __FILE__, __LINE__); throw runtime_error (buffer); }
#define     TEST_XYOP(x, y, op) { bool result_xyop = ((x) op (y)); cerr << (result_xyop ? "PASS" : "FAIL") << ": " #x " " #op " " #y " (" << (x) << " " #op " " << (y) << ")" << endl; TEST_ASSERTION(result_xyop); }
#define     TEST_XYF(x, y, p) TEST_XYOP(fabs (x - y), p, <=)
#define     TEST_XY(x, y) TEST_XYOP(x, y, ==)
#define		TEST_CASE(name)										            \
			class Test##name {									            \
				public:											            \
					Test##name () {								            \
						try {									            \
						    cerr << "STARTING: " #name << endl;             \
							test ();				                        \
							cerr << "PASSED: " #name << endl << endl;	    \
						} catch (exception& exception) {		            \
							cerr << "    " << exception.what () << endl;    \
							exit (EXIT_FAILURE);                            \
						}										            \
					}											            \
					void test ();								            \
        			static Test##name test##name;			                \
			};													            \
        	Test##name Test##name::test##name;			                    \
			inline void Test##name::test ()
