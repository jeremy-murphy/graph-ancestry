#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "algorithms.hpp"

#include <vector>

using namespace std;
using namespace jwm;

BOOST_AUTO_TEST_SUITE(algorithms)

BOOST_AUTO_TEST_CASE(test_representative_element)
{
    vector<unsigned> a, b;
    auto bi = back_inserter(b);
    auto result = representative_element(begin(a), a.size(), bi);
    BOOST_CHECK_EQUAL(b.size(), 0);
    
    for(unsigned i = 10; i < 20; i++)
    {
        a.push_back(i);
        a.push_back(i);
    }
    
    result = representative_element(begin(a), a.size(), bi);
    vector<size_t> expected = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    BOOST_CHECK_EQUAL(expected.size(), b.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(expected), end(expected), begin(b), end(b));
}

BOOST_AUTO_TEST_SUITE_END()
