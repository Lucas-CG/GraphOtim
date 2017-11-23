#ifndef INSTANCES_HPP
#define INSTANCES_HPP


#include "Graph.hpp"
#include <vector>
#include <utility>

//instance 1

void generateInstance1(Graph & instance1Graph, std::vector< std::pair<uint_fast32_t, uint_fast32_t> > instance1RequestedConnections){

  std::unordered_set<uint_fast32_t> dummyfrequency;

  instance1Graph.addEdge(1, 2, dummyfrequency);
  instance1Graph.addEdge(2, 3, dummyfrequency);
  instance1Graph.addEdge(3, 4, dummyfrequency);
  instance1Graph.addEdge(2, 5, dummyfrequency);
  instance1Graph.addEdge(3, 5, dummyfrequency);
  instance1Graph.addEdge(5, 6, dummyfrequency);
  instance1Graph.addEdge(6, 9, dummyfrequency);
  instance1Graph.addEdge(8, 9, dummyfrequency);
  instance1Graph.addEdge(7, 8, dummyfrequency);
  instance1Graph.addEdge(1, 7, dummyfrequency);

  std::pair<uint_fast32_t, uint_fast32_t> instance1Connection1(1, 4);
  std::pair<uint_fast32_t, uint_fast32_t> instance1Connection2(2, 9);
  std::pair<uint_fast32_t, uint_fast32_t> instance1Connection3(5, 8);
  instance1RequestedConnections.push_back(instance1Connection1);
  instance1RequestedConnections.push_back(instance1Connection2);
  instance1RequestedConnections.push_back(instance1Connection3);

}


#endif
