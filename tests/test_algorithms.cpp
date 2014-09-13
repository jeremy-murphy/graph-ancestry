#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "algorithms.hpp"

#include <vector>
#include <boost/iterator/indirect_iterator.hpp>

using namespace std;
using namespace jwm;

BOOST_AUTO_TEST_SUITE(algorithms)

BOOST_AUTO_TEST_CASE(test_representative_element)
{
    vector<unsigned> a;
    vector<typename vector<unsigned>::const_iterator> b;
    auto bi = back_inserter(b);
    auto result = representative_element(begin(a), a.size(), bi);
    BOOST_CHECK_EQUAL(b.size(), 0);
    
    for(unsigned i = 10; i < 20; i++)
    {
        a.push_back(i);
        a.push_back(i);
    }
    
    result = representative_element(begin(a), a.size(), bi);
    vector<typename vector<unsigned>::const_iterator> const expected = {begin(a), begin(a) + 2, begin(a) + 4, begin(a) + 6, begin(a) + 8, begin(a) + 10, begin(a) + 12, begin(a) + 14, begin(a) + 16, begin(a) + 18};
    BOOST_CHECK_EQUAL(expected.size(), b.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(expected)), boost::make_indirect_iterator(end(expected)), boost::make_indirect_iterator(begin(b)), boost::make_indirect_iterator(end(b)));
}

BOOST_AUTO_TEST_SUITE_END()
