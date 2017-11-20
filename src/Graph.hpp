#include <unordered_map> //std::unordered_map
#include <utility> //std::pair
#include <cstdint> //uint_fast32_t
#include <string> //std::string
#include <priority_queue> //std::priority_queue

typedef uint_fast32_t nodeIndex;
typedef uint_fast32_t frequencyIndex;

template<typename Out>
void split(const std::string &s, char delim, Out result);

class Path {

  private:
    std::vector<nodeIndex> nodeList;

  public:
    frequencyIndex frequency;

}

class Graph {

  private:

    //o grafo é representado como um array associativo
    //o primeiro elemento é um vértice, e o segundo elemento é um outro array associativo
    //com vizinhos do vértice e os números das frequências alocadas para as respectivas arestas
    std::unordered_map< nodeIndex, std::unordered_map < nodeIndex, frequencyIndex > > list;

  public:

    void addEdge(nodeIndex a, nodeIndex b, frequencyIndex f);
    void changeFrequency(nodeIndex a, nodeIndex b, frequencyIndex f);
    void readFromFile(std::string fileName);
    void print();
    std::priority_queue < std::vector<nodeIndex> > kShortestPaths(uint_fast32_t num_shortest_paths);

    //iterar sobre unordered_map:
    //for (auto it: unordered_map_name){it.algo...}

    Graph();


}
