#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "lsca.hpp"

#include <boost/graph/adjacency_list.hpp>

#include <iostream>

#ifndef NDEBUG
#include <boost/graph/graphviz.hpp>
#include <fstream>
#endif

using namespace std;
using namespace graph_algorithms;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); cerr.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

struct Fischer_2010_fig1
{
    boost::adjacency_list<> g;
    
    Fischer_2010_fig1()
    {
        boost::add_edge(0, 1, g);
        boost::add_edge(0, 2, g);
        boost::add_edge(1, 3, g);
        boost::add_edge(1, 4, g);
        boost::add_edge(2, 3, g);
        boost::add_edge(2, 4, g);
        boost::add_edge(2, 5, g);
        boost::add_edge(2, 6, g);
        boost::add_edge(5, 8, g);
        boost::add_edge(6, 7, g);
        boost::add_edge(6, 8, g);
        boost::add_edge(6, 9, g);
        boost::add_edge(7, 9, g);
    }
};


BOOST_FIXTURE_TEST_CASE(empty_preprocess, Fischer_2010_fig1)
{
#ifndef NDEBUG
    ofstream output("Fischer_2010_fig1.dot");
    boost::write_graphviz(output, g);
#endif
    boost::adjacency_list<> h;
    LSCA_preprocess(g, h);
}
