#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "RMQ.hpp"

#include <vector>
#include <boost/iterator/indirect_iterator.hpp>

using namespace std;

struct basic_8
{
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9, 1};
    vector<unsigned> A = {2, 6, 6, 4, 4, 5, 1, 2, 4, 4, 4, 1, 1};
};


struct basic_7
{
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9};
    vector<unsigned> A = {2, 6, 6, 4, 4, 5, 2, 4, 4, 4};
};


BOOST_FIXTURE_TEST_SUITE(TEST_RMQ, basic_8)

BOOST_AUTO_TEST_CASE(test_basic)
{
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    RMQ::sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(A), end(A), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}

BOOST_FIXTURE_TEST_CASE(test_7, basic_7)
{
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    RMQ::sparse_table(begin(Q), end(Q), B);
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(A), end(A), boost::make_indirect_iterator(begin(B)), boost::make_indirect_iterator(end(B)));
}


BOOST_AUTO_TEST_SUITE_END()
