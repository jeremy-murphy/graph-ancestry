### graph-algorithms

Generic graph (and tree) algorithms.

And their supporting general algorithms.

#### Requirements
* Boost.
  * Concept     - concept checking
  * MultiArray  - Sparse Tree data structure
  * Graph       - graph/tree algorithms and data structures
  * Test, Timer - unit tests and measurements

#### Contents
1. Graph algorithms
  1. **lowest single common ancestor** (LSCA) - *incomplete*
2. Tree algorithms
 1. **lowest common ancestor** (LCA), <*Θ*(*n* lg *n*), *Θ*(1)>
   * By reduction to RMQ.
3. General algorithms
  1. **range minimum query** (RMQ), <*Θ*(*n* lg *n*), *Θ*(1)>
    * The *Sparse Table* implementation.

#### Early results
On large inputs (4 million integers) RMQ preprocesses in ~0.8 seconds and queries in ~15 nanoseconds on my humble Intel Core i5 mobile.
