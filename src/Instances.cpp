#include "Graph.hpp"
#include <vector>
#include <utility>

//instance 1

Graph generateInstance1(std::vector< std::pair<int, int> > & instance1RequestedConnections){

  Graph graph(9);


  graph.addEdge(0, 1);
  graph.addEdge(1, 2);
  graph.addEdge(2, 3);
  graph.addEdge(1, 4);
  graph.addEdge(2, 4);
  graph.addEdge(4, 5);
  graph.addEdge(5, 8);
  graph.addEdge(7, 8);
  graph.addEdge(6, 7);
  graph.addEdge(0, 6);


  std::pair<int, int> instance1Connection1(0, 3);
  std::pair<int, int> instance1Connection2(1, 8);
  std::pair<int, int> instance1Connection3(4, 7);
  instance1RequestedConnections.push_back(instance1Connection1);
  instance1RequestedConnections.push_back(instance1Connection2);
  instance1RequestedConnections.push_back(instance1Connection3);


  return graph;

}
