#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "algorithms.hpp"

#include <boost/iterator/indirect_iterator.hpp>

#include <vector>
#include <locale>
#include <iostream>
#include <list>
#include <unordered_map>
#include <map>

using namespace std;
using namespace general;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); cerr.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

typedef typename vector<unsigned>::const_iterator const_iterator;

struct basic_unordered_map
{
    vector<unsigned> a = {0, 0, 1, 0, 2, 1, 2, 1};
    unordered_map<unsigned, size_t> expected_indices = {{0, 0}, {2, 4}, {1, 2}};
};


BOOST_AUTO_TEST_CASE(test_lower_log2)
{
    vector<unsigned> const q = {1, 2, 3, 4, 5, 100, 1000, 10000};
    vector<unsigned> const a = {0, 1, 1, 2, 2, 6, 9, 13};
    for(unsigned i = 0; i < q.size(); i++)
        BOOST_CHECK_EQUAL(lower_log2(q[i]), a[i]);
}


BOOST_AUTO_TEST_CASE(test_upper_log2)
{
    vector<unsigned> const q = {1, 2, 3, 4, 5, 7, 8};
    vector<unsigned> const a = {0, 1, 2, 2, 3, 3, 3};
    for(unsigned i = 0; i < q.size(); i++)
        BOOST_CHECK_EQUAL(upper_log2(q[i]), a[i]);
}
