#ifndef SPLITTINGHEURISTIC_HPP
#define SPLITTINGHEURISTIC_HPP

#include "Graph.hpp"
#include <vector> //std::vector

class SplittingHeuristic{

  public:

      intType getMinDistanceVertex(std::vector<intType> vec)
      Path dijkstraForSplitting(Graph graph, intType source, intType destination);
      Graph generateCollisionGraph(Graph graph, std::vector<Path> pathList);
      intType greedyColoring(Graph collisionGraph);
      intType calculate(Graph graph, std::vector< std::pair<intType, intType> > & requestedConnections)

};

class CompareDegrees
{
  public:
    bool operator()(std::pair<intType,intType> n1, std::pair<intType,intType> n2) {
        return n1.second < n2.second;
    }
};

#endif
