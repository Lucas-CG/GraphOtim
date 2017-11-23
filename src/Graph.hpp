#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <map> //std::map
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair
#include <cstdint> //uint_fast32_t
#include <string> //std::string
#include <vector> //std::vector

template<typename Out>
void split(const std::string &s, char delim, Out result);


class Path
{

  public:
    uint_fast32_t frequency;
    std::vector<uint_fast32_t> nodeList;


};

bool doPathsHaveCollision(Path &path1, Path &path2);


class Graph
{

  public:

    //o grafo é representado como um array associativo
    //o primeiro elemento é um vértice, e o segundo elemento é um outro array associativo
    //com vizinhos do vértice e os números das frequências alocadas para as respectivas arestas
    std::map< uint_fast32_t, std::map < uint_fast32_t, std::unordered_set<uint_fast32_t> > > list;

    void addEdge(uint_fast32_t a, uint_fast32_t b, std::unordered_set<uint_fast32_t> frequencies);
    void addFrequency(uint_fast32_t a, uint_fast32_t b, uint_fast32_t f);

    uint_fast32_t numEdges = 0;

    //iterar sobre map:
    //for (auto it: map_name){*it.algo...}

    Graph();


};

#endif
