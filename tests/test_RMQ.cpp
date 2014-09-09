#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "RMQ.hpp"

#include <vector>
#include <iterator>
#include <boost/iterator/indirect_iterator.hpp>

using namespace std;

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
    typedef typename decltype(Q)::const_iterator const_iterator;
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
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    RMQ::preprocess_sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(A)), boost::make_indirect_iterator(end(A)), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}

BOOST_FIXTURE_TEST_CASE(test_7, basic_7)
{
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    RMQ::preprocess_sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(A)), boost::make_indirect_iterator(end(A)), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}


BOOST_FIXTURE_TEST_CASE(test_16, basic_16)
{
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    RMQ::preprocess_sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(A)), boost::make_indirect_iterator(end(A)), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}


BOOST_AUTO_TEST_CASE(basic_query)
{
    auto q = RMQ::translate(1, 2, Q.size());
    BOOST_CHECK_EQUAL(q, 8);

    q = RMQ::translate(1, 3, Q.size());
    BOOST_CHECK_EQUAL(q, 13);

    auto foo = RMQ::query_sparse_table(1, 5, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 4);

    foo = RMQ::query_sparse_table(0, 7, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 1);

    foo = RMQ::query_sparse_table(0, 0, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 2);

    foo = RMQ::query_sparse_table(0, 6, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 2);
}

/*
BOOST_AUTO_TEST_CASE(invalid_query)
{
    BOOST_REQUIRE_THROW(RMQ::query_sparse_table(1, 0, Q.size(), A), invalid_argument);
    BOOST_REQUIRE_THROW(RMQ::query_sparse_table(1, 100, Q.size(), A), invalid_argument);
}
*/

BOOST_FIXTURE_TEST_CASE(query_16, basic_16)
{
    auto foo = RMQ::query_sparse_table(0, 5, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 2);

    foo = RMQ::query_sparse_table(0, 15, Q.size(), A);
    BOOST_CHECK_EQUAL(*foo, 1);
}

BOOST_AUTO_TEST_SUITE_END()
