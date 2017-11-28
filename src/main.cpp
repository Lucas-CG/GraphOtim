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

int main(int argc, char **argv)
{

  startTime = std::chrono::high_resolution_clock::now();
  endTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> endTimeSpan;



  BranchAndBound bb;
  LocalUpperLimit localUpperLimit;
  LocalLowerLimit localLowerLimit;
  std::vector< std::pair<int, int> > connectionsToDo;


  BetweennessHeuristic bh;
  SplittingHeuristic sh;

  std::vector< std::unordered_set<int> > frequencies;


  std::vector< std::pair<int, int> > requestedConnections;
  Graph graph = generateInstance1(requestedConnections);



  startTime = std::chrono::high_resolution_clock::now();

  bb.run(graph, requestedConnections, frequencies, 0, connectionsToDo, localUpperLimit, localLowerLimit);
  int bbResult = bb.bestSolutionValue;

  endTime = std::chrono::high_resolution_clock::now();

  endTimeSpan = std::chrono::duration_cast<std::chrono::duration<double> >(endTime - startTime);

  printf("branch-and-bound: %lf segundos\n", endTimeSpan.count());
  std::cout << "Resultado do branch-and-bound: " << bbResult << std::endl;





  startTime = std::chrono::high_resolution_clock::now();
  int shResult = sh.calculate(graph, requestedConnections);
  endTime = std::chrono::high_resolution_clock::now();

  printf("heuristica de divisao em roteamento e coloracao: %lf segundos\n", endTimeSpan.count());
  std::cout << "Resultado: " << shResult << std::endl;





  startTime = std::chrono::high_resolution_clock::now();
  int bhResult = bh.calculate(graph, requestedConnections);
  endTime = std::chrono::high_resolution_clock::now();

  printf("heuristica de betweenness: %lf segundos\n", endTimeSpan.count());
  std::cout << "Resultado: " << bhResult << std::endl;



  return 0;

}
