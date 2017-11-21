#include <Graph.hpp>
#include <vector>
#include <utility>

class BranchAndBound
{

  private:

  public:

    void run(Graph g, std::vector< std::pair<intType, intType> > requestedConnections,
      std::vector< std::unordered_set<intType> > frequencies, intType position, intType &solutionValue,
      std::vector< std::unordered_set<intType> > bestSolution);

    bool isViable(Graph g, std::vector< std::pair<intType, intType> > requestedConnections,
       std::vector< std::unordered_set<intType> > frequencies);

    bool findPath(Graph &graph, intType source, intType destination, intType frequency, Path &presentPath);

}
