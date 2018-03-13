# Graph
## [2-SAT](./Codes/Graph/2-SAT.cpp): 
  * 1-based implementation of 2-Satisfiability Problem (positive numbers for positive propositions and negative number for negative propositions.

## [Connected Components](./Codes/Graph/ConnectedComponnents.cpp)
  * <b>Note</b>: Need to extract necesary code for each of the following subproblems.
  * StronglyConnectedComponents
  * BiconnectedComponents
  * ArticulationPointsAndBridges

## [Count Number of Minimum Spanning Trees](./Codes/Graph/CountMinimumSpanningTree.cpp)
  * This is uses Count Number of Spanning Trees and [mint](cjtoribio/Algorithms/Codes/Math/ModInt.cpp#L68) 

## [Count Number of Spanning Trees](./Codes/Graph/CountSpanningTree.cpp)
  * <b>Note</b>: Code is equivalent to building the matrix with addEdge and finding determinant. [mint](cjtoribio/Algorithms/Codes/Math/ModInt.cpp#L68) is just a ModularInt with the defined operations (/, *, +, -) [note modular division needed]

## [Eulerian Path And Cycle](./Codes/Graph/EulerianPathAndCycle) 
  * Finds an eulerian path or a cycle in O(N)

## [HopcroftCarp](./Codes/Graph/EulerianPathAndCycle)
  * Maximum matching O(sqrt(V)*E)

## [Hungarian Algorithm](./Graph/MaxFlow%5BDinics-EdgeList%5D.cpp)
  * Finds a <b>weighted</b> maximum matching in O(V^3) best for dense graphs, for non complete graphs use INF as edge cost, an alternative is to run MinCostMaxFlow.

## [MaxFlow](./Codes/Graph/MincostMaxflow%5BAdjMatrix%5D.cpp)
  * Has a running time of O(|V|^2 |E|). 
  * Can be used to find MaximumMatching and runs relatively fast, in theory same complexity as HopcroftCarp if all edges has capacity 1. 

## [MinCostMaxFlow](./Codes/Graph/MincostMaxflow%5BAdjMatrix%5D.cpp)
  * Can be a substitute of HungarianAlgorithm and also other MaxFlow with costs involved.

## [VertextCover](./Codes/Graph/VertexCover.cpp)
  * Can find a minimum vertex cover after a maximum matching is provided. 
  * NOTE: It does not have MaxMatching you have to run MaxMatching and then pass the matching to this algorithm.
