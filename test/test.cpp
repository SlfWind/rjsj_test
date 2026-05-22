#define BOOST_TEST_MODULE rjsj_exercise_tests

#include <boost/test/unit_test.hpp>

#include <functional>
#include <forward_list>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <set>
#include <vector>

#include "exercise.h"

BOOST_AUTO_TEST_SUITE(exercise_wb_tests)

BOOST_AUTO_TEST_CASE(exercise_wb_1_outputs_expected_branch_result)
{
    std::ostringstream captured;
    std::streambuf *original = std::cout.rdbuf(captured.rdbuf());

    exercise_wb_1(3, 2, 1);

    std::cout.rdbuf(original);

    BOOST_TEST(captured.str() == "3\n");
}

BOOST_AUTO_TEST_SUITE_END()
