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

BOOST_AUTO_TEST_CASE(test_element_indexer)
{
    vector<char> const a = {'a', 'b', 'c', 'b', 'b', 'a', 'z', 'c'};
    vector<pair<char, ptrdiff_t>> b;
    unordered_map<char, ptrdiff_t> c;
    decltype(b) const expected_b = {{'a', 0}, {'b', 1}, {'c', 2}, {'b', 3}, {'b', 4}, {'a', 5}, {'z', 6}, {'c', 7}};
    decltype(c) const expected_c = {{'a', 0}, {'b', 1}, {'c', 2}, {'z', 6}};
    transform(begin(a), end(a), back_inserter(b), element_index<decltype(begin(a))>());
    BOOST_CHECK(b == expected_b);
    transform(begin(a), end(a), inserter(c, end(c)), element_index<decltype(begin(a))>());
    BOOST_CHECK(c == expected_c);
}
