### graph-algorithms

Generic graph ancestry algorithms.

And their supporting general algorithms.

#### Requirements
* Boost.
  * Concept     - concept checking
  * MultiArray  - Sparse Tree data structure
  * Graph       - graph/tree algorithms and data structures
  * Test, Timer - unit tests and measurements

#### Contents
1. Graph algorithms
 1. **lowest common ancestor** (LCA), <*Θ*(*n* lg *n*), *Θ*(1)>
   * By reduction to RMQ.
 2. **common ancestor existence**, <*O*(*|V|⋅|E|*), *Θ*(1)>
   * Efficient when querying multiple vertices.
2. Graph visitors
   1. **eulerian_path** outputs Eulerian path of a graph.
   2. **vertex_depth** outputs vertex depth (for same sequence as Eulerian path).
3. General algorithms
  1. **range minimum query** (RMQ), <*Θ*(*n* lg *n*), *Θ*(1)>
    * The *Sparse Table* implementation.

#### Early results
On a moderately large input (4 million integers) RMQ preprocesses in ~0.402 seconds (96 ns per element).

On a smaller input (1 million vertices), LCA preprocesses in ~0.883 seconds (842 ns per element).

Queries for both are in ~7-16 nanoseconds.

Measured on my humble Intel Core i5 mobile.
