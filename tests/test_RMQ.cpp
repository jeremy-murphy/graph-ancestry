#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "RMQ.hpp"

#include <vector>
#include <iterator>
#include <boost/iterator/indirect_iterator.hpp>
#include <locale>
#include <iostream>

using namespace std;
using namespace general;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

struct basic_8
{
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9, 1};
    typedef typename decltype(Q)::const_iterator const_iterator;
    vector<const_iterator> A = {begin(Q), begin(Q) + 2, begin(Q) + 2, begin(Q) + 4, begin(Q) + 4, begin(Q) + 5, begin(Q) + 7, begin(Q), begin(Q) + 4, begin(Q) + 4, begin(Q) + 4, begin(Q) + 7, begin(Q) + 7};
};


struct basic_7
{
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9};
    vector<unsigned*> A = {&Q[0], &Q[2], &Q[2], &Q[4], &Q[4], &Q[5], &Q[0], &Q[4], &Q[4], &Q[4]};
};

struct basic_16
{
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9, 1, 10, 11, 3, 7, 19, 4, 11, 16};
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> A = {
        begin(Q), begin(Q) + 2, begin(Q) + 2, begin(Q) + 4, begin(Q) + 4, begin(Q) + 5, begin(Q) + 7, begin(Q) + 7, begin(Q) + 8, begin(Q) + 10, begin(Q) + 10, begin(Q) + 11, begin(Q) + 13, begin(Q) + 13, begin(Q) + 14,
        // 4
        begin(Q), begin(Q) + 4, begin(Q) + 4, begin(Q) + 4, begin(Q) + 7, begin(Q) + 7, begin(Q) + 7, begin(Q) + 7, begin(Q) + 10, begin(Q) + 10, begin(Q) + 10, begin(Q) + 13, begin(Q) + 13,
        // 8
        begin(Q) + 7, begin(Q) + 7, begin(Q) + 7, begin(Q) + 7, begin(Q) + 7, begin(Q) + 7, begin(Q) + 7, begin(Q) + 7, begin(Q) + 10,
        // 16
        begin(Q) + 7
        };
};

BOOST_FIXTURE_TEST_SUITE(TEST_RMQ, basic_8)

BOOST_AUTO_TEST_CASE(test_basic)
{
    vector<const_iterator> B;
    preprocess_sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(A)), boost::make_indirect_iterator(end(A)), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}

BOOST_FIXTURE_TEST_CASE(test_7, basic_7)
{
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    preprocess_sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(A)), boost::make_indirect_iterator(end(A)), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}


BOOST_FIXTURE_TEST_CASE(test_16, basic_16)
{
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    preprocess_sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(A)), boost::make_indirect_iterator(end(A)), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}


BOOST_AUTO_TEST_CASE(basic_query)
{
    auto q = translate_sparse_table(1, 2, Q.size());
    BOOST_CHECK_EQUAL(q, 8);

    q = translate_sparse_table(1, 3, Q.size());
    BOOST_CHECK_EQUAL(q, 13);

    auto foo = query_sparse_table(1, 5, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 4);

    foo = query_sparse_table(0, 7, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 1);

    foo = query_sparse_table(0, 0, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 2);

    foo = query_sparse_table(0, 6, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 2);
}


BOOST_FIXTURE_TEST_CASE(query_16, basic_16)
{
    auto foo = query_sparse_table(0, 5, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 2);

    foo = query_sparse_table(0, 15, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 1);
}

BOOST_AUTO_TEST_SUITE_END()


#ifdef NDEBUG

#include <boost/function_output_iterator.hpp>
#include <algorithm>
#include <random>
#include <boost/timer/timer.hpp>
#include <fstream>
#include <string>

struct huge_random_RMQ
{
    mt19937 engine;
    uniform_int_distribution<unsigned> d;
    vector<unsigned> a;
    typedef typename vector<unsigned>::const_iterator const_iterator;
    
    huge_random_RMQ() : d(0, (1ul << 32) - 1)
    {
        fill_n(back_inserter(a), 1ul << 17, d(engine));
    }
};

BOOST_FIXTURE_TEST_SUITE(measure_RMQ, huge_random_RMQ)

BOOST_AUTO_TEST_CASE(measure_preprocess)
{
    auto const j = 1u << 8;
    boost::timer::cpu_timer timer;
    for(unsigned i = 0; i < j; i++)
    {
        vector<const_iterator> B;
        preprocess_sparse_table(begin(a), end(a), B);
    }
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
