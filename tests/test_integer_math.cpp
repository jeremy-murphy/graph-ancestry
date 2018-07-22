#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <integer_math.hpp>

#include <boost/iterator/indirect_iterator.hpp>

#include <boost/unordered_map.hpp>

#include <vector>
#include <locale>
#include <iostream>
#include <list>
#include <unordered_map>
#include <map>

using namespace boost;
using namespace general;

struct enable_locale
{
    enable_locale() { std::cout.imbue(std::locale("")); std::cerr.imbue(std::locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

typedef std::vector<int>::const_iterator const_iterator;

struct basic_unordered_map
{
    std::vector<int> a = {0, 0, 1, 0, 2, 1, 2, 1};
    boost::unordered_map<int, size_t> expected_indices = {{0, 0}, {2, 4}, {1, 2}};
};


BOOST_AUTO_TEST_CASE(test_lower_log2)
{
    std::vector<int> const q = {1, 2, 3, 4, 5, 100, 1000, 10000};
    std::vector<int> const a = {0, 1, 1, 2, 2, 6, 9, 13};
    for (unsigned i = 0; i < q.size(); i++)
        BOOST_CHECK_EQUAL(lower_log2(q[i]), a[i]);
}


BOOST_AUTO_TEST_CASE(test_upper_log2)
{
    std::vector<int> const q = {1, 2, 3, 4, 5, 7, 8};
    std::vector<int> const a = {0, 1, 2, 2, 3, 3, 3};
    for (unsigned i = 0; i < q.size(); i++)
        BOOST_CHECK_EQUAL(upper_log2(q[i]), a[i]);
}
