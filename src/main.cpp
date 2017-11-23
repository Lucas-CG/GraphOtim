#include "Graph.hpp"
#include "BetweennessHeuristic.hpp"
#include "SplittingHeuristic.hpp"
#include "BranchAndBound.hpp"
#include <vector>
#include <utility>
#include <chrono>
#include <iostream>
#include "Instances.hpp"


std::chrono::high_resolution_clock::time_point startTime;
std::chrono::high_resolution_clock::time_point endTime;

int main()
{

  startTime = std::chrono::high_resolution_clock::now();
  endTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> endTimeSpan;


  BranchAndBound bb;
  BetweennessHeuristic bh;
  SplittingHeuristic sh;

  std::vector< std::unordered_set<uint_fast32_t> > frequencies;


  Graph graph;
  std::vector< std::pair<uint_fast32_t, uint_fast32_t> > requestedConnections;
  generateInstance1(graph, requestedConnections);

  startTime = std::chrono::high_resolution_clock::now();

  bb.run(graph, requestedConnections, frequencies, 0, requestedConnections);
  uint_fast32_t bbResult = bb.bestSolutionValue;

  endTime = std::chrono::high_resolution_clock::now();

  endTimeSpan = std::chrono::duration_cast<std::chrono::duration<double> >(endTime - startTime);

  printf("branch-and-bound: %lf segundos\n", endTimeSpan.count());
  std::cout << "Resultado do branch-and-bound: " << bbResult << std::endl;


  startTime = std::chrono::high_resolution_clock::now();
  uint_fast32_t shResult = sh.calculate(graph, requestedConnections);
  endTime = std::chrono::high_resolution_clock::now();

  printf("heuristica de divisao em roteamento e coloracao: %lf segundos\n", endTimeSpan.count());
  std::cout << "Resultado: " << shResult << std::endl;


  uint_fast32_t bhResult;

  startTime = std::chrono::high_resolution_clock::now();
  bool dummy = bh.calculate(graph, requestedConnections, bhResult);
  endTime = std::chrono::high_resolution_clock::now();

  printf("heuristica de betweenness: %lf segundos\n", endTimeSpan.count());
  std::cout << "Resultado: " << bhResult << std::endl;



}
