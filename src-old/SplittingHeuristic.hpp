#ifndef SPLITTINGHEURISTIC_HPP
#define SPLITTINGHEURISTIC_HPP

#include "Graph.hpp"
#include <vector> //std::vector

class SplittingHeuristic{

  public:

      int getMinDistanceVertex(std::vector<int> vec);
      Path dijkstraForSplitting(Graph graph, int source, int destination);
      Graph generateCollisionGraph(Graph graph, std::vector<Path> pathList);
      std::vector<Path> generateShortestPathsForRequestedConnections(Graph graph, std::vector< std::pair<int, int> > & requestedConnections);
      int greedyColoring(Graph collisionGraph);
      int calculate(Graph graph, std::vector< std::pair<int, int> > & requestedConnections);

};

class CompareDegrees
{
  public:
    bool operator()(std::pair<int,int> n1, std::pair<int,int> n2) {
        return n1.second < n2.second;
    }
};

#endif
