#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <map> //std::map
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair
#include <cstdint> //int
#include <string> //std::string
#include <vector> //std::vector

template<typename Out>
void split(const std::string &s, char delim, Out result);


class Path
{

  public:
    int frequency;
    std::vector<int> nodeList;


};

bool doPathsHaveCollision(Path &path1, Path &path2);
bool doPathsHaveFrequencyCollision(Path &path1, Path &path2);


class Graph
{

  public:

    //o grafo é representado como um array associativo
    //o primeiro elemento é um vértice, e o segundo elemento é um outro array associativo
    //com vizinhos do vértice e os números das frequências alocadas para as respectivas arestas
    //std::map< int, std::map < int, std::unordered_set<int> > > list;

    std::vector< std::vector< std::pair< bool, std::unordered_set<int> > > > matrix;

    void addEdge(int a, int b);
    void addFrequency(int a, int b, int f);

    int numEdges = 0;
    int numVertices = 0;

    //iterar sobre map:
    //for (auto it: map_name){*it.algo...}

    Graph(int size);


};

#endif
