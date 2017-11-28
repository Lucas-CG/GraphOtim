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

Graph generateInstance2(std::vector< std::pair<int, int> > & instance1RequestedConnections){

  Graph graph(20);


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
  graph.addEdge(19, 17);
  graph.addEdge(10, 11);
  graph.addEdge(6, 12);
  graph.addEdge(16, 15);
  graph.addEdge(0, 10);
  graph.addEdge(0, 13);
  graph.addEdge(14, 15);
  graph.addEdge(14, 19);
  graph.addEdge(15, 19);
  graph.addEdge(5, 18);
  graph.addEdge(1, 5);
  graph.addEdge(7, 18);
  graph.addEdge(9, 15);
  graph.addEdge(6, 18);
  graph.addEdge(6, 17);
  graph.addEdge(6, 16);
  graph.addEdge(10, 13);
  graph.addEdge(15, 18);
  graph.addEdge(0, 9);
  graph.addEdge(0, 5);
  graph.addEdge(6, 11);
  graph.addEdge(14, 18);


  std::pair<int, int> instance1Connection1(0, 3);
  std::pair<int, int> instance1Connection2(1, 8);
  std::pair<int, int> instance1Connection3(4, 7);
  instance1RequestedConnections.push_back(instance1Connection1);
  instance1RequestedConnections.push_back(instance1Connection2);
  instance1RequestedConnections.push_back(instance1Connection3);


  return graph;

}
