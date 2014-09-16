### graph-algorithms

Generic graph (and tree) algorithms.

And their supporting general algorithms.

#### Requirements
* Boost.
  * Graph       - for graph/tree algorithms
  * Test, Timer - for the tests

#### Contents
1. Graph algorithms
  1. lowest common single ancestor (LSCA) - *incomplete*
2. Tree algorithms
 1. lowest common ancestor (LCA),  Θ(*n* lg *n*), Θ(1)>
   * By reduction to the RMQ problem.
3. General algorithms
  1. range minimum query (RMQ), <Θ(*n* lg *n*), Θ(1)>
    * The *Sparse Table* implementation.
  2. representative_element, average-case Θ(*n*)
    * Returns an iterator to the first occurence of each distinct value in the input range.
    * Requires the input value_type to be hashable.
