#ifndef BRANCHANDBOUND_HPP
#define BRANCHANDBOUND_HPP

#include "Graph.hpp"
#include <vector> //std::vector
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair

class Limit
{

  public:

    void calculate();

    bool isViable = false;
    int value;

};

class LowerLimit: public Limit{};
class UpperLimit: public Limit{};
class GlobalLowerLimit: public Limit{};
class GlobalUpperLimit: public Limit{};
class LocalLowerLimit: public Limit{};
class LocalUpperLimit: public Limit{};
class PathGlobalUpperLimit: public Limit{};

class BranchAndBound
{
  public:

    void run(Graph &graph, std::vector< std::pair<int, int> > & requestedConnections,
       std::vector< std::unordered_set<int> > frequencies, int frequencyIndex,
       std::vector< std::pair<int, int> > connectionsToDo);

    bool findPath(Graph &graph, int source, int destination, int frequency, Path &presentPath);

    int generateSuperiorLimit(Graph &graph);

    int generateInferiorLimit(Graph &graph);

    Graph makeFrequencyGraph(Graph graph, std::vector< std::unordered_set<int> > &frequencies);

    bool checkConnection(Graph graph, std::pair<int, int> connection, Path & path);

    void checkConnections(Graph graph, std::vector< std::pair<int, int> > & requestedConnections,
       std::vector< std::unordered_set<int> > & frequencies, std::vector< std::pair<int, int> > &connectionsToDo);

    static GlobalUpperLimit globalUpperLimit;
    static GlobalLowerLimit globalLowerLimit;
    static PathGlobalUpperLimit pathLimit;
    static int maxFrequencies;
    static std::vector< std::unordered_set<int> > &bestSolution;
    static int &bestSolutionValue;

};

#endif
