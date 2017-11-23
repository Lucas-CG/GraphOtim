#include "Graph.hpp"
#include "BetweennessHeuristic.hpp"
#include "SplittingHeuristic.hpp"
#include "BranchAndBound.hpp"
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>
#include "Instances.hpp"


chrono::high_resolution_clock::time_point startTime;
chrono::high_resolution_clock::time_point endTime;

int main()
{

  startTime = chrono::high_resolution_clock::now();
  endTime = chrono::high_resolution_clock::now();
  chrono::duration<double> endTimeSpan;


  BranchAndBound bb;
  BetweennessHeuristic bh;
  SplittingHeuristic sh;

  std::vector< std::unordered_set<intType> > frequencies;

  Graph graph = instance1Graph;
  std::vector< std::pair<intType, intType> > requestedConnections = instance1RequestedConnections;

  startTime = chrono::high_resolution_clock::now();

  bb.run(graph, requestedConnections, frequencies, 0, requestedConnections);
  intType bbResult = bb.bestSolutionValue;

  endTime = chrono::high_resolution_clock::now();

  endTimeSpan = chrono::duration_cast<chrono::duration<double> >(endTime - startTime);

  printf("branch-and-bound: %lf segundos\n", endTimeSpan.count());
  std::cout << "Resultado do branch-and-bound: " << bbResult << std::endl;


  startTime = chrono::high_resolution_clock::now();
  intType shResult = sh.calculate(graph, requestedConnections);
  endTime = chrono::high_resolution_clock::now();

  printf("heuristica de divisao em roteamento e coloracao: %lf segundos\n", endTimeSpan.count());
  std::cout << "Resultado: " << shResult << std::endl;


  intType bhResult;

  startTime = chrono::high_resolution_clock::now();
  bool dummy = bh.calculate(graph, requestedConnections, bhResult);
  endTime = chrono::high_resolution_clock::now();

  printf("heuristica de betweenness: %lf segundos\n", endTimeSpan.count());
  std::cout << "Resultado: " << bhResult << std::endl;



}
