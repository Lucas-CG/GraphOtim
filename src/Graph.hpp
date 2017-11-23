#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map> //std::unordered_map
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair
#include <cstdint> //uint_fast32_t
#include <string> //std::string

#define INT_TYPE_MAX UINT_FAST32_MAX
#define INT_TYPE_MIN UINT_FAST32_MIN
typedef uint_fast32_t intType;

template<typename Out>
void split(const std::string &s, char delim, Out result);


class Path
{

  public:
    intType frequency;
    std::vector<intType> nodeList;


};

bool doPathsHaveCollision(Path &path1, Path &path2);


class Graph
{

  public:

    //o grafo é representado como um array associativo
    //o primeiro elemento é um vértice, e o segundo elemento é um outro array associativo
    //com vizinhos do vértice e os números das frequências alocadas para as respectivas arestas
    std::unordered_map< intType, std::unordered_map < intType, std::unordered_set<intType> > > list;

    void addEdge(intType a, intType b, std::unordered_set<intType> frequencies);
    void addFrequency(intType a, intType b, intType f);
    void readFromFile(std::string fileName);

    intType numEdges = 0;

    //iterar sobre unordered_map:
    //for (auto it: unordered_map_name){*it.algo...}

    Graph();


};

#endif
