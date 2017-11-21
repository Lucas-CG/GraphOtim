#include <Graph.hpp>
#include <vector> //std::vector
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair

class BranchAndBound
{

  private:

  public:

    void run(Graph & graph, std::vector< std::pair<intType, intType> > & requestedConnections,
      std::vector< std::unordered_set<intType> > frequencies, intType frequencyIndex,
      std::vector< std::unordered_set<intType> > &bestSolution, intType & bestSolutionValue);

    bool isViable(Graph & graph, std::vector< std::pair<intType, intType> > & requestedConnections,
       std::vector< std::unordered_set<intType> > & frequencies);

    bool findPath(Graph &graph, intType source, intType destination, intType frequency, Path &presentPath);

    bool doPathsHaveCollision(Path & path1, Path & path2);

    intType generateSuperiorLimit(Graph &graph);

    intType generateInferiorLimit(Graph &graph);

    bool promising(Graph &graph, )

    static globalUpperLimit;
    static globalLowerLimit;

}
