#pragma once

// this file will be included in all
#ifdef		_TEST_

#define     TEST_ASSERTION(x) if (not (X)) throw "ASSERTION FAILURE"
#define     TEST_XYOP(x, y, op) { bool result_xyop = ((x) op (y)); cerr << (result_xyop ? "PASS" : "FAIL") << ": " #x " " #op " " #y " (" << (x) << " " #op " " << (y) << ")" << endl; TEST_ASSERTION(result_xyop); }
#define     TEST_XYF(x, y, p) TESTXYOP(fabs (x - y), p, <=)
#define     TEST_XY(x, y) TESTXYOP(x, y, ==)
#define		TEST_CASE(name)										\
			class Test##name {									\
				public:											\
					Test##name () {								\
						try {									\
							TestFunction#name ();				\
							cerr << "PASS: Test##name" << endl;	\
						} catch (exception& exception) {		\
							cerr << exception.what () << endl;	\
						}										\
					}											\
					void test ();								\
			};													\
			Test##name test##name;								\
			inline void Test##name::test ()

int main (int argc, const char** argv) {
	return EXIT_SUCCESS;
}

#else	//	_TEST_

#define     TEST_ASSERTION(x)
#define     TEST_XYOP(x, y, op)
#define     TEST_XYF(x, y, p)
#define     TEST_XY(x, y)
#define		TEST_CASE(name)										\


#endif	//	_TEST_
