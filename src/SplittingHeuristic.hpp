#ifndef SPLITTINGHEURISTIC_HPP
#define SPLITTINGHEURISTIC_HPP

#include "Graph.hpp"
#include <vector> //std::vector

class SplittingHeuristic{

  public:

      uint_fast32_t getMinDistanceVertex(std::vector<uint_fast32_t> vec);
      Path dijkstraForSplitting(Graph graph, uint_fast32_t source, uint_fast32_t destination);
      Graph generateCollisionGraph(Graph graph, std::vector<Path> pathList);
      std::vector<Path> generateShortestPathsForRequestedConnections(Graph graph, std::vector< std::pair<uint_fast32_t, uint_fast32_t> > & requestedConnections);
      uint_fast32_t greedyColoring(Graph collisionGraph);
      uint_fast32_t calculate(Graph graph, std::vector< std::pair<uint_fast32_t, uint_fast32_t> > & requestedConnections);

};

class CompareDegrees
{
  public:
    bool operator()(std::pair<uint_fast32_t,uint_fast32_t> n1, std::pair<uint_fast32_t,uint_fast32_t> n2) {
        return n1.second < n2.second;
    }
};

#endif
