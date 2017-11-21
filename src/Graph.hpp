#include <unordered_map> //std::unordered_map
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair
#include <cstdint> //uint_fast32_t
#include <string> //std::string
#include <priority_queue> //std::priority_queue

typedef uint_fast32_t intType;

template<typename Out>
void split(const std::string &s, char delim, Out result);

class Path
{

  public:
    intType frequency;
    std::vector<intType> nodeList;

}

class Graph
{

  private:

    //o grafo é representado como um array associativo
    //o primeiro elemento é um vértice, e o segundo elemento é um outro array associativo
    //com vizinhos do vértice e os números das frequências alocadas para as respectivas arestas
    std::unordered_map< intType, std::unordered_map < intType, std::unordered_set<intType> > > list;

  public:

    void addEdge(intType a, intType b, std::unordered_set<intType> frequencies);
    void addFrequency(intType a, intType b, intType f);
    void readFromFile(std::string fileName);
    void print();
    std::priority_queue < std::vector<intType> > kShortestPaths(uint_fast32_t num_shortest_paths);

    intType numEdges = 0;

    //iterar sobre unordered_map:
    //for (auto it: unordered_map_name){*it.algo...}

    Graph();


}
