#ifndef SPLITTINGHEURISTIC_HPP
#define SPLITTINGHEURISTIC_HPP

#include "Graph.hpp"
#include <vector> //std::vector
#include <utility> //std::pair
#include <unordered_set> //std::unordered_set

class SplittingHeuristic
{

  public:

      int getMinDistanceVertex(std::vector<int> dist, std::unordered_set<int> S_);
      Path dijkstraForSplitting(Graph graph, int source, int destination);
      std::vector<Path> generateShortestPathsForRequestedConnections(Graph graph, std::vector< std::pair<int, int> > & requestedConnections);
      Graph generateCollisionGraph(Graph graph, std::vector<Path> pathList);
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
