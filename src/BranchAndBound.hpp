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
    uint_fast32_t value;

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

    void run(Graph &graph, std::vector< std::pair<uint_fast32_t, uint_fast32_t> > & requestedConnections,
       std::vector< std::unordered_set<uint_fast32_t> > frequencies, uint_fast32_t frequencyIndex,
       std::vector< std::pair<uint_fast32_t, uint_fast32_t> > connectionsToDo);

    bool findPath(Graph &graph, uint_fast32_t source, uint_fast32_t destination, uint_fast32_t frequency, Path &presentPath);

    uint_fast32_t generateSuperiorLimit(Graph &graph);

    uint_fast32_t generateInferiorLimit(Graph &graph);

    Graph makeFrequencyGraph(Graph graph, std::vector< std::unordered_set<uint_fast32_t> > &frequencies);

    bool checkConnection(Graph graph, std::pair<uint_fast32_t, uint_fast32_t> connection, Path & path);

    void checkConnections(Graph graph, std::vector< std::pair<uint_fast32_t, uint_fast32_t> > & requestedConnections,
       std::vector< std::unordered_set<uint_fast32_t> > & frequencies, std::vector< std::pair<uint_fast32_t, uint_fast32_t> > &connectionsToDo);

    static GlobalUpperLimit globalUpperLimit;
    static GlobalLowerLimit globalLowerLimit;
    static PathGlobalUpperLimit pathLimit;
    static uint_fast32_t maxFrequencies;
    static std::vector< std::unordered_set<uint_fast32_t> > &bestSolution;
    static uint_fast32_t &bestSolutionValue;

};

#endif
