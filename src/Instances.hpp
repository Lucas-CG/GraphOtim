#ifndef INSTANCES_HPP
#define INSTANCES_HPP


#include "Graph.hpp"
#include <vector>
#include <utility>

//instance 1

Graph instance1Graph;
std::vector< std::pair<intType, intType> > instance1RequestedConnections;

instance1Graph.addEdge(1, 2);
instance1Graph.addEdge(2, 3);
instance1Graph.addEdge(3, 4);
instance1Graph.addEdge(2, 5);
instance1Graph.addEdge(3, 5);
instance1Graph.addEdge(5, 6);
instance1Graph.addEdge(6, 9);
instance1Graph.addEdge(8, 9);
instance1Graph.addEdge(7, 8);
instance1Graph.addEdge(1, 7);

std::pair<intType, intType> instance1Connection1(1, 4);
std::pair<intType, intType> instance1Connection2(2, 9);
std::pair<intType, intType> instance1Connection3(5, 8);
instance1RequestedConnections.push_back(instance1Connection1);
instance1RequestedConnections.push_back(instance1Connection2);
instance1RequestedConnections.push_back(instance1Connection3);



#endif
