#ifndef BETWEENNESSHEURISTIC_HPP
#define BETWEENNESSHEURISTIC_HPP

#include "Graph.hpp" //Graph, int
#include <utility> //std::pair
#include <vector> //std::vector
#include <unordered_set> //std::unordered_set

class BetweennessHeuristic
{

  public:

    int getMinDistanceVertex(std::vector<int> vec, std::unordered_set<int> S_);

    Path dijkstraForBetweenness(Graph graph, int source, int destination);

    int calculate(Graph &graph, std::vector< std::pair<int, int> > & requestedConnections);

};

#endif
