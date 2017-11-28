#ifndef INSTANCES_HPP
#define INSTANCES_HPP


#include "Graph.hpp"
#include <vector>
#include <utility>

//instance 1

Graph generateInstance1(std::vector< std::pair<int, int> > & instance1RequestedConnections){

  Graph graph(9);

  graph.addEdge(1, 2);
  graph.addEdge(2, 3);
  graph.addEdge(3, 4);
  graph.addEdge(2, 5);
  graph.addEdge(3, 5);
  graph.addEdge(5, 6);
  graph.addEdge(6, 9);
  graph.addEdge(8, 9);
  graph.addEdge(7, 8);
  graph.addEdge(1, 7);

  std::pair<int, int> instance1Connection1(1, 4);
  std::pair<int, int> instance1Connection2(2, 9);
  std::pair<int, int> instance1Connection3(5, 8);
  instance1RequestedConnections.push_back(instance1Connection1);
  instance1RequestedConnections.push_back(instance1Connection2);
  instance1RequestedConnections.push_back(instance1Connection3);

}


#endif
