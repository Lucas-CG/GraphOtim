#ifndef BETWEENNESSHEURISTIC_HPP
#define BETWEENNESSHEURISTIC_HPP

#include "Graph.hpp" //Graph, int
#include <utility> //std::pair
#include <vector> //std::vector

class BetweennessHeuristic
{

  public:

    int getMinDistanceVertex(std::vector<int> vec);

    Path dijkstraForBetweenness(Graph graph, int source, int destination);

    int calculate(Graph &graph, std::vector< std::pair<int, int> > & requestedConnections);

};

#endif
