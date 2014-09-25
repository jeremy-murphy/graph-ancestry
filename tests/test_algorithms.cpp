#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "algorithms.hpp"

#include <boost/iterator/indirect_iterator.hpp>

#include <vector>
#include <locale>
#include <iostream>
#include <list>

using namespace std;
using namespace general;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

typedef typename vector<unsigned>::const_iterator const_iterator;

struct basic
{
    vector<unsigned> a = {0, 0, 1, 0, 2, 1, 2, 1};
    vector<const_iterator> expected = {begin(a), begin(a) + 2, begin(a) + 4};
};


BOOST_AUTO_TEST_CASE(empty_input)
{
    vector<unsigned> empty;
    vector<const_iterator> b;
    auto bi = back_inserter(b);
    representative_element(begin(empty), empty.size(), bi);
    BOOST_CHECK(b.empty());

    representative_element(begin(empty), end(empty), bi);
    BOOST_CHECK(b.empty());
}


BOOST_AUTO_TEST_CASE(test_log2)
{
    vector<unsigned> const q = {1, 2, 3, 100, 1000, 10000};
    vector<unsigned> const a = {0, 1, 1, 6, 9, 13};
    for(unsigned i = 0; i < q.size(); i++)
        BOOST_CHECK_EQUAL(general::log2(q[i]), a[i]);
}

BOOST_FIXTURE_TEST_SUITE(test_representative_element, basic)

BOOST_AUTO_TEST_CASE(test_basic_n)
{
    vector<const_iterator> b;
    auto bi = back_inserter(b);
    representative_element(begin(a), a.size(), bi);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(expected)), boost::make_indirect_iterator(end(expected)), boost::make_indirect_iterator(begin(b)), boost::make_indirect_iterator(end(b)));
}


BOOST_AUTO_TEST_CASE(test_basic_range)
{
    vector<const_iterator> b;
    auto bi = back_inserter(b);
    representative_element(begin(a), end(a), bi);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(expected)), boost::make_indirect_iterator(end(expected)), boost::make_indirect_iterator(begin(b)), boost::make_indirect_iterator(end(b)));
}


BOOST_AUTO_TEST_CASE(test_representative_element_list)
{
    typedef list<unsigned> input_type;
    input_type const input(begin(a), end(a));
    vector<typename input_type::const_iterator> b;
    auto bi = back_inserter(b);
    representative_element(begin(input), end(input), bi);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(expected)), boost::make_indirect_iterator(end(expected)), boost::make_indirect_iterator(begin(b)), boost::make_indirect_iterator(end(b)));
}


BOOST_AUTO_TEST_SUITE_END()


/*      And now... unit measurements!    */

#ifdef NDEBUG

#include <boost/function_output_iterator.hpp>
#include <algorithm>
#include <random>
#include <boost/timer/timer.hpp>
#include <fstream>
#include <string>

struct huge_random
{
    mt19937 engine;
    uniform_int_distribution<unsigned> d;
    vector<unsigned> a;
    
    huge_random() : d(0, (1ul << 32) - 1)
    {
        fill_n(back_inserter(a), 1ul << 20, d(engine));
    }
};

BOOST_FIXTURE_TEST_SUITE(measure_algorithms, huge_random)

BOOST_AUTO_TEST_CASE(measure_representative_element)
{
    auto null_output = boost::make_function_output_iterator([](const_iterator){});
    auto const j = 1u << 8;
    boost::timer::cpu_timer timer;
    for(unsigned i = 0; i < j; i++)
        representative_element(begin(a), end(a), null_output);
    timer.stop();
    double const t = timer.elapsed().user + timer.elapsed().system;
    auto const t_n = t / a.size() / j;
    string const test_name = string(boost::unit_test::framework::current_test_case().p_name);
    string const data_file(string(".") + test_name);
    BOOST_MESSAGE(test_name << "[" << j << " × " << a.size() << "]: " << t_n << " ns per element");
    ifstream foo(data_file);
    if(foo.is_open())
    {
        double t0;
        foo >> t0;
        auto const r = t_n / t0 - 1.0;  // regression, as a fraction of 100.
        BOOST_REQUIRE_MESSAGE(r < 0.10, r * 100.0 << "% regression!");
        BOOST_WARN_MESSAGE(r < 0.05, r * 100.0 << "% regression.");
    }
    else
    {
        BOOST_MESSAGE(data_file << ": not found; starting anew with " << t_n << ".");
        ofstream output(data_file);
        output << t_n << "\n";
    }
}

BOOST_AUTO_TEST_SUITE_END()

#endif
