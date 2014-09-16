#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "algorithms.hpp"

#include <vector>
#include <boost/iterator/indirect_iterator.hpp>


using namespace std;
using namespace jwm;

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

BOOST_AUTO_TEST_SUITE_END()


/*      And now... unit measurements!    */

#ifdef NDEBUG

#include <boost/function_output_iterator.hpp>
#include <algorithm>
#include <random>
#include <boost/timer/timer.hpp>
#include <fstream>
#include <string>
#include <iostream>

struct huge_random
{
    mt19937 engine;
    uniform_int_distribution<unsigned> d;
    vector<unsigned> a;
    
    huge_random() : d(0, (1ul << 32) - 1)
    {
        fill_n(back_inserter(a), 1ul << 28, d(engine));
    }
};

BOOST_FIXTURE_TEST_SUITE(measure_representative_element, huge_random)

BOOST_AUTO_TEST_CASE(measure_sequence)
{
    auto null_output = boost::make_function_output_iterator([](const_iterator){});
    boost::timer::auto_cpu_timer timer;
    representative_element(begin(a), end(a), null_output);
    timer.stop();
    auto const t = timer.elapsed().user + timer.elapsed().system;
    string const data_file(string(".") + string(boost::unit_test::framework::current_test_case().p_name));
    ifstream foo(data_file);
    if(foo.is_open())
    {
        boost::timer::nanosecond_type t0;
        foo >> t0;
        auto const r = double(t) / double(t0);
        cerr << "r: " << r << "\n";
        BOOST_REQUIRE_MESSAGE(r < 1.10, (r - 1) * 100.0 << "% regression!");
        BOOST_WARN_MESSAGE(r < 1.05, (r - 1) * 100.0 << "% regression (just a warning).");
    }
    else
    {
        BOOST_MESSAGE(data_file << ": not found; starting anew with " << t << ".");
        ofstream foo(data_file);
        foo << t << "\n";
    }
}

BOOST_AUTO_TEST_SUITE_END()

#endif
