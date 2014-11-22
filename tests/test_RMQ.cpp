#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "RMQ.hpp"

#include <boost/mpl/vector.hpp>

#include <array>
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

struct iterator_8
{
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9, 1};
    typedef typename decltype(Q)::const_iterator const_iterator;
    vector<const_iterator> A = {begin(Q), begin(Q) + 2, begin(Q) + 2, begin(Q) + 4, begin(Q) + 4, begin(Q) + 5, begin(Q) + 7, begin(Q), begin(Q) + 4, begin(Q) + 4, begin(Q) + 4, begin(Q) + 7, begin(Q) + 7};
};


struct iterator_7
{
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9};
    vector<unsigned*> A = {&Q[0], &Q[2], &Q[2], &Q[4], &Q[4], &Q[5], &Q[0], &Q[4], &Q[4], &Q[4]};
};

struct iterator_16
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

BOOST_FIXTURE_TEST_SUITE(RMQ_iterator, iterator_8)

BOOST_AUTO_TEST_CASE(test_basic)
{
    vector<const_iterator> B;
    preprocess_sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(A)), boost::make_indirect_iterator(end(A)), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}

BOOST_FIXTURE_TEST_CASE(test_7, iterator_7)
{
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    preprocess_sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(A)), boost::make_indirect_iterator(end(A)), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}


BOOST_FIXTURE_TEST_CASE(test_16, iterator_16)
{
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    preprocess_sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(A)), boost::make_indirect_iterator(end(A)), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}


BOOST_AUTO_TEST_CASE(basic_query)
{
    auto q = translate_sparse_table(1u, 2u, Q.size());
    BOOST_CHECK_EQUAL(q, 8);

    q = translate_sparse_table(1u, 3u, Q.size());
    BOOST_CHECK_EQUAL(q, 13);

    auto foo = query_sparse_table(1u, 5u, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 4);

    foo = query_sparse_table(0u, 7u, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 1);

    foo = query_sparse_table(0u, 0u, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 2);

    foo = query_sparse_table(0u, 6u, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 2);
}


BOOST_FIXTURE_TEST_CASE(query_16, iterator_16)
{
    auto foo = query_sparse_table(0u, 5u, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 2);

    foo = query_sparse_table(0u, 15u, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 1);
}

BOOST_AUTO_TEST_SUITE_END()


#include <boost/multi_array.hpp>

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
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9};
    vector<size_t> A = {0, 2, 2, 4, 4, 5, 0, 4, 4, 4};
};

struct index_16
{
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9, 1, 10, 11, 3, 7, 19, 4, 11, 16};
    vector<size_t> A = {0, 2, 2, 4, 4, 5, 7, 7, 8, 10, 10, 11, 13, 13, 14, // 2
                        0, 4, 4, 4, 7, 7, 7, 7, 10, 10, 10, 13, 13, // 4
                        7, 7, 7, 7, 7, 7, 7, 7, 10, // 8
                        7 // 16
    };
};


BOOST_AUTO_TEST_SUITE(RMQ_index)

BOOST_AUTO_TEST_CASE(test_empty_preprocess)
{
    vector<unsigned> Q;
    boost::multi_array<size_t, 2> B(boost::extents[0][0]);
    boost::multi_array<size_t, 2> const C(B);
    index_preprocess_sparse_table(Q, B);
    BOOST_CHECK(B == C);
    Q.push_back(0);
    index_preprocess_sparse_table(Q, B);
    BOOST_CHECK(B == C);
    Q.push_back(1);
    index_preprocess_sparse_table(Q, B);
    BOOST_CHECK(B == C);
}

BOOST_FIXTURE_TEST_SUITE(RMQ_index_8, index_8)

// typedef boost::mpl::vector<index_8, index_7, index_16> test_fixtures;


BOOST_AUTO_TEST_CASE(test_preprocess)
{
    boost::multi_array<size_t, 2> C(M);
    index_preprocess_sparse_table(A, M);
    /*
    for (auto q = 1u; q <=j; q++)
    {
        for (auto p = 0u; p < i; p++)
            cerr << M[q][p] << " ";
        cerr << endl;
    }
    */
}

BOOST_FIXTURE_TEST_CASE(test_query, index_8)
{
    index_preprocess_sparse_table(A, M);
    auto minimum = index_query_sparse_table(1u, 5u, A, M);
    BOOST_CHECK_EQUAL(minimum, 4);
    
    minimum = index_query_sparse_table(0u, 7u, A, M);
    BOOST_CHECK_EQUAL(minimum, 7);
    
    minimum = index_query_sparse_table(0u, 0u, A, M);
    BOOST_CHECK_EQUAL(minimum, 0);
    
    minimum = index_query_sparse_table(0u, 6u, A, M);
    BOOST_CHECK_EQUAL(minimum, 0);
}

BOOST_AUTO_TEST_SUITE_END()
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
    vector<const_iterator> B;
    auto const f = [&]()
    {
        preprocess_sparse_table(begin(a), end(a), B);
        B.clear();
    };
    measure(a.size(), 1u << 4, f);
}

BOOST_AUTO_TEST_CASE(measure_RMQ_index_preprocess)
{
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<size_t, 2> M((boost::extents[range(1, 23)][1 << 22ul]));
    auto const f = [&]()
    {
        index_preprocess_sparse_table(a, M);
    };
    measure(a.size(), 1u << 4, f);
}


BOOST_AUTO_TEST_CASE(measure_RMQ_query)
{
    vector<const_iterator> B;
    preprocess_sparse_table(begin(a), end(a), B);
    auto const f = bind(query_sparse_table<size_t, size_t, decltype(B)>, 0, a.size() - 1, a.size(), B);
    measure(1, 1ul << 29, f, "query");
}

BOOST_AUTO_TEST_CASE(measure_RMQ_index_query)
{
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<size_t, 2> M((boost::extents[range(1, 23)][1 << 22ul]));
    index_preprocess_sparse_table(a, M);
    auto const f = bind(index_query_sparse_table<size_t, decltype(a), decltype(M)>, 0, a.size() - 1, a, M);
    measure(1, 1ul << 29, f, "query");
}

BOOST_AUTO_TEST_SUITE_END()

#endif
