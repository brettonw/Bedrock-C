#ifndef     UNIT_TESTER_H_
#define     UNIT_TESTER_H_

// during development, the framework is not present, so in order to have
// intellisense work in eclipse you have to bring in the framework...
#ifndef     BOOST_TEST_MODULE
#define     BOOST_TEST_MODULE ts_chipper_unit_test
#include    <boost/test/unit_test.hpp>
#endif  //  BOOST_TEST_MODULE

#define     ASSERT_CALL(x) BOOST_CHECK(x)
#define     TESTXYOP(x, y, op) BOOST_TEST_MESSAGE("CHECK: " #x " " #op " " #y " (" << (x) << " " #op " " << (y) << "): " << (((x) op (y)) ? "PASS" : "FAIL\n    ")); BOOST_CHECK((x) op (y))
#define     TESTXYF(x, y, p) TESTXYOP(fabs (x - y), p, <=)
#define     TESTXY(x, y) TESTXYOP(x, y, ==)

#endif  //  UNIT_TESTER_H_
