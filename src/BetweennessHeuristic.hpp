#include "Graph.hpp" //Graph, intType
#include <utility> //std::pair
#include <vector> //std::vector

class BetweennessHeuristic
{

  public:

    intType getMinDistanceVertex(std::vector<intType> vec)

    Path dijkstraForBetweenness(Graph graph, intType source, intType destination);

    //falso se algum caminho não for encontrado
    bool calculate(Graph &graph, std::vector< std::pair<intType, intType> > & requestedConnections, intType & value);

}
