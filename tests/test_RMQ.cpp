#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "RMQ.hpp"

#include <vector>

using namespace std;

struct basic
{
    vector<unsigned> Q = {2, 7, 6, 8, 4, 5, 9, 1};
    vector<unsigned> A = {2, 6, 6, 4, 4, 5, 1, 2, 4, 4, 4, 1, 1};
};

BOOST_FIXTURE_TEST_SUITE(TEST_RMQ, basic)

BOOST_AUTO_TEST_CASE(test_basic)
{
    typedef typename vector<unsigned>::const_iterator const_iterator;
    vector<const_iterator> B;
    RMQ::sparse_table(Q, B);
    
}

BOOST_AUTO_TEST_SUITE_END()
