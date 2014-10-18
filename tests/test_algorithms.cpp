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

using namespace std;
using namespace general;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

typedef typename vector<unsigned>::const_iterator const_iterator;

struct basic_unordered_map
{
    vector<unsigned> a = {0, 0, 1, 0, 2, 1, 2, 1};
    unordered_map<unsigned, size_t> expected_indices = {{0, 0}, {2, 4}, {1, 2}};
};

struct basic_vector
{
    vector<unsigned> a = {0, 0, 1, 0, 2, 1, 2, 1};
    vector<unsigned> expected_indices = {0, 2, 4};
};


BOOST_AUTO_TEST_CASE(test_log2)
{
    vector<unsigned> const q = {1, 2, 3, 100, 1000, 10000};
    vector<unsigned> const a = {0, 1, 1, 6, 9, 13};
    for(unsigned i = 0; i < q.size(); i++)
        BOOST_CHECK_EQUAL(general::log2(q[i]), a[i]);
}


BOOST_FIXTURE_TEST_CASE(test_representative_element_map, basic_unordered_map)
{
    decltype(expected_indices) result;
    representative_element(a, result);
    BOOST_CHECK(result == expected_indices);
}


BOOST_FIXTURE_TEST_CASE(test_representative_element_vector, basic_vector)
{
    decltype(expected_indices) result(3); // Need to allocate space in advance.
    representative_element(a, result);
    BOOST_CHECK(result == expected_indices);
}


/*      And now... unit measurements!    */

#ifdef NDEBUG

#include "measurement.hpp"

#include <algorithm>
#include <random>

template <size_t N>
struct huge_random
{
    mt19937 engine;
    uniform_int_distribution<unsigned> d;
    vector<unsigned> a;
    vector<size_t> b;
    
    huge_random() : d(0, N - 1), b(N)
    {
        auto gen = bind(d, engine);
        generate_n(back_inserter(a), 1ul << 20, gen);
    }
};

BOOST_FIXTURE_TEST_SUITE(measure_algorithms, huge_random<1ul << 10>)

BOOST_AUTO_TEST_CASE(measure_representative_element_time)
{
    auto f = bind(representative_element<decltype(a), decltype(b)>, a, b);
    measure(a.size(), 1u << 8, f);
}

BOOST_AUTO_TEST_CASE(measure_representative_element_space)
{
    mt19937 engine;
    uniform_int_distribution<unsigned> d;
    auto gen = bind(d, engine);

    for(unsigned long i = 1; i != 1ul << 20; i *= 2)
    {
        vector<char> a(i);
        generate(begin(a), end(a), gen);
        // auto const seen = representative_element(a, b);
        // BOOST_MESSAGE(i << ": " << seen.size());
    }
}


BOOST_AUTO_TEST_SUITE_END()

#endif
