#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "RMQ.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/multi_array.hpp>

#include <array>
#include <vector>
#include <iterator>
#include <locale>
#include <iostream>

using namespace std;
using namespace general;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);


struct index_8
{
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<size_t, 2> M;
    array<unsigned, 8> A{{2, 7, 6, 8, 4, 5, 9, 1}};
    vector<size_t> Z = {0, 2, 2, 4, 4, 5, 7, 0, 4, 4, 4, 7, 7};
    
    index_8() : M(boost::extents[range(1, 4)][8]) {}
};

struct index_7
{
    vector<unsigned> A = {2, 7, 6, 8, 4, 5, 9};
    vector<size_t> Z = {0, 2, 2, 4, 4, 5, 0, 4, 4, 4};
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<size_t, 2> M;
    index_7() : M(boost::extents[range(1, 3)][7]) {}
};

struct index_16
{
    vector<unsigned> A = {2, 7, 6, 8, 4, 5, 9, 1, 10, 11, 3, 7, 19, 4, 11, 16};
    vector<size_t> Z = {0, 2, 2, 4, 4, 5, 7, 7, 8, 10, 10, 11, 13, 13, 14, // 2
                        0, 4, 4, 4, 7, 7, 7, 7, 10, 10, 10, 13, 13, // 4
                        7, 7, 7, 7, 7, 7, 7, 7, 10, // 8
                        7 // 16
    };
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<size_t, 2> M;
    index_16() : M(boost::extents[range(1, 5)][16]) {}
};


BOOST_AUTO_TEST_SUITE(RMQ_index)

BOOST_AUTO_TEST_CASE(test_empty_preprocess)
{
    vector<unsigned> Q;
    boost::multi_array<size_t, 2> B(boost::extents[0][0]);
    boost::multi_array<size_t, 2> const C(B);
    preprocess_sparse_table(Q, B);
    BOOST_CHECK(B == C);
    Q.push_back(0);
    preprocess_sparse_table(Q, B);
    BOOST_CHECK(B == C);
    Q.push_back(1);
    preprocess_sparse_table(Q, B);
    BOOST_CHECK(B == C);
}

BOOST_FIXTURE_TEST_SUITE(RMQ_index_8, index_8)

// typedef boost::mpl::vector<index_8, index_7, index_16> test_fixtures;


BOOST_AUTO_TEST_CASE(test_preprocess)
{
    boost::multi_array<size_t, 2> C(M);
    preprocess_sparse_table(A, M);
    /*
    for (auto q = 1u; q <=j; q++)
    {
        for (auto p = 0u; p < i; p++)
            cerr << M[q][p] << " ";
        cerr << endl;
    }
    */
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_CASE(test_query8, index_8)
{
    preprocess_sparse_table(A, M);
    auto minimum = query_sparse_table(1u, 5u, A, M);
    BOOST_CHECK_EQUAL(minimum, 4);
    minimum = query_sparse_table(0u, 7u, A, M);
    BOOST_CHECK_EQUAL(minimum, 7);
    minimum = query_sparse_table(0u, 0u, A, M);
    BOOST_CHECK_EQUAL(minimum, 0);
    minimum = query_sparse_table(0u, 6u, A, M);
    BOOST_CHECK_EQUAL(minimum, 0);
}

BOOST_FIXTURE_TEST_CASE(test_query7, index_7)
{
    preprocess_sparse_table(A, M);
    auto minimum = query_sparse_table(1u, 5u, A, M);
    BOOST_CHECK_EQUAL(minimum, 4);
    minimum = query_sparse_table(5u, 6u, A, M);
    BOOST_CHECK_EQUAL(minimum, 5);
    minimum = query_sparse_table(0u, 0u, A, M);
    BOOST_CHECK_EQUAL(minimum, 0);
    minimum = query_sparse_table(0u, 6u, A, M);
    BOOST_CHECK_EQUAL(minimum, 0);
}

BOOST_FIXTURE_TEST_CASE(test_query16, index_16)
{
    preprocess_sparse_table(A, M);
    auto minimum = query_sparse_table(1u, 5u, A, M);
    BOOST_CHECK_EQUAL(minimum, 4);
    minimum = query_sparse_table(5u, 6u, A, M);
    BOOST_CHECK_EQUAL(minimum, 5);
    minimum = query_sparse_table(0u, 0u, A, M);
    BOOST_CHECK_EQUAL(minimum, 0);
    minimum = query_sparse_table(0u, 6u, A, M);
    BOOST_CHECK_EQUAL(minimum, 0);
    minimum = query_sparse_table(2u, 3u, A, M);
    BOOST_CHECK_EQUAL(minimum, 2);
    minimum = query_sparse_table(6u, 15u, A, M);
    BOOST_CHECK_EQUAL(minimum, 7);
    minimum = query_sparse_table(8u, 12u, A, M);
    BOOST_CHECK_EQUAL(minimum, 10);
    minimum = query_sparse_table(0u, 15u, A, M);
    BOOST_CHECK_EQUAL(minimum, 7);
}

BOOST_AUTO_TEST_SUITE_END()


#ifdef NDEBUG

#include "measurement.hpp"

#include <algorithm>
#include <random>

struct random_RMQ
{
    mt19937 engine;
    uniform_int_distribution<unsigned> d;
    vector<unsigned> a;
    typedef typename vector<unsigned>::const_iterator const_iterator;
    
    random_RMQ() : d(0, (1ul << 32) - 1)
    {
        fill_n(back_inserter(a), 1ul << 22, d(engine));
    }
};

BOOST_FIXTURE_TEST_SUITE(measure_RMQ, random_RMQ)

BOOST_AUTO_TEST_CASE(measure_RMQ_preprocess)
{
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<size_t, 2> M((boost::extents[range(1, 23)][1 << 22ul]));
    auto const f = [&]()
    {
        preprocess_sparse_table(a, M);
    };
    measure(a.size(), 1u << 4, f);
}


BOOST_AUTO_TEST_CASE(measure_RMQ_query)
{
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<size_t, 2> M((boost::extents[range(1, 23)][1 << 22ul]));
    preprocess_sparse_table(a, M);
    auto const f = bind(query_sparse_table<size_t, decltype(a), decltype(M)>, 0, a.size() - 1, a, M);
    measure(1, 1ul << 29, f, "query");
}

BOOST_AUTO_TEST_SUITE_END()

#endif
