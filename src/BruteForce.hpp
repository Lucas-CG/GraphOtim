#include <Graph.hpp>
#include <vector>
#include <utility>

class BruteForce
{

  private:

  public:

    void run(Graph g, std::vector< std::pair<intType, intType> > requestedConnections,
      std::vector<intType> frequencies, intType position, intType &solutionValue,
      std::vector<intType> bestSolution);

    bool isViable(Graph g, std::vector< std::pair<intType, intType> > requestedConnections,
       std::vector<intType> frequencies);

    bool findPath(Graph &graph, intType source, intType destination, intType frequency, Path &presentPath);

}
