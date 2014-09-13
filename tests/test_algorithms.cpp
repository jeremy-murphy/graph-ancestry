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
    BOOST_CHECK_EQUAL(empty.size(), 0);
}


BOOST_FIXTURE_TEST_SUITE(test_representative_element, basic)

BOOST_AUTO_TEST_CASE(test_basic_n)
{
    vector<const_iterator> b;
    auto bi = back_inserter(b);
    representative_element(begin(a), a.size(), bi);
    BOOST_CHECK_EQUAL(expected.size(), b.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(expected)), boost::make_indirect_iterator(end(expected)), boost::make_indirect_iterator(begin(b)), boost::make_indirect_iterator(end(b)));
}


BOOST_AUTO_TEST_CASE(test_basic_range)
{
    vector<const_iterator> b;
    auto bi = back_inserter(b);
    representative_element(begin(a), end(a), bi);
    BOOST_CHECK_EQUAL(expected.size(), b.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(boost::make_indirect_iterator(begin(expected)), boost::make_indirect_iterator(end(expected)), boost::make_indirect_iterator(begin(b)), boost::make_indirect_iterator(end(b)));
}

BOOST_AUTO_TEST_SUITE_END()
