#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "transformers.hpp"

#include <vector>
#include <locale>
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace general;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); cerr.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

BOOST_AUTO_TEST_CASE(test_reflection)
{

}
