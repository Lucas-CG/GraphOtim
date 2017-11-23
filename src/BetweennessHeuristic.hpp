#ifndef BETWEENNESSHEURISTIC_HPP
#define BETWEENNESSHEURISTIC_HPP

#include "Graph.hpp" //Graph, uint_fast32_t
#include <utility> //std::pair
#include <vector> //std::vector

class BetweennessHeuristic
{

  public:

    uint_fast32_t getMinDistanceVertex(std::vector<uint_fast32_t> vec);

    Path dijkstraForBetweenness(Graph graph, uint_fast32_t source, uint_fast32_t destination);

    //falso se algum caminho não for encontrado
    bool calculate(Graph &graph, std::vector< std::pair<uint_fast32_t, uint_fast32_t> > & requestedConnections, uint_fast32_t & value);

};

#endif
