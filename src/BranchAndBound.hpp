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
    intType value;

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

    void run(Graph & graph, std::vector< std::pair<intType, intType> > & requestedConnections,
      std::vector< std::unordered_set<intType> > frequencies, intType frequencyIndex,
      std::vector< std::unordered_set<intType> > &bestSolution, intType & bestSolutionValue);

    bool isViable(Graph & graph, std::vector< std::pair<intType, intType> > & requestedConnections,
       std::vector< std::unordered_set<intType> > & frequencies);

    bool findPath(Graph &graph, intType source, intType destination, intType frequency, Path &presentPath);

    intType generateSuperiorLimit(Graph &graph);

    intType generateInferiorLimit(Graph &graph);

    bool promising(Graph &graph, )

    static GlobalUpperLimit globalUpperLimit;
    static GlobalLowerLimit globalLowerLimit;
    static PathGlobalUpperLimit pathLimit;
    static intType maxFrequencies;
    static std::vector< std::unordered_set<intType> > &bestSolution;
    static intType &bestSolutionValue;

};

#endif
