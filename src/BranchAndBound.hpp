#ifndef BRANCHANDBOUND_HPP
#define BRANCHANDBOUND_HPP

#include "Graph.hpp"
#include <vector> //std::vector
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair
#include <climits> //INT_MAX


class Limit
{

  public:

    bool isViable;
    int value;

};

class LowerLimit: public Limit{};
class UpperLimit: public Limit{};
class GlobalLowerLimit: public Limit{};
class GlobalUpperLimit: public Limit{};
class LocalLowerLimit: public Limit{};
class LocalUpperLimit: public Limit{};
class PathGlobalUpperLimit: public Limit{

  public:
    void calculate(Graph & graph);

};

class BranchAndBound
{
  public:

    void run(Graph &graph, std::vector< std::pair<int, int> > & requestedConnections,
       std::vector< std::unordered_set<int> > frequencies, int frequencyIndex,
       int connectionsToDo, LocalUpperLimit localUpperLimit, LocalLowerLimit localLowerLimit);

    bool findPath(Graph &graph, int source, int destination, int frequency, Path &presentPath);

    Graph makeFrequencyGraph(Graph graph, std::vector< std::unordered_set<int> > &frequencies);

    bool checkConnection(Graph & graph, std::pair<int, int> connection, std::vector<Path> & pathVector);

    void checkConnections(Graph & graph, std::vector< std::pair<int, int> > & requestedConnections,
       std::vector< std::unordered_set<int> > & frequencies, int &connectionsToDo);

    GlobalUpperLimit globalUpperLimit;
    GlobalLowerLimit globalLowerLimit;
    PathGlobalUpperLimit pathLimit;
    int maxFrequencies;
    std::vector< std::unordered_set<int> > bestSolution;
    int bestSolutionValue = INT_MAX;

};

#endif
