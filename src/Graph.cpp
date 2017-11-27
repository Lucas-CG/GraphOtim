#include "Graph.hpp"
#include <fstream> //std::ifstream
#include <iterator> //std::back_inserter
#include <string> //std::string, std::stoi
#include <unordered_set> //std::unordered_set

template<typename Out>
void split(const std::string &s, char delim, Out result)
{

    std::stringstream ss;
    ss.str(s);
    std::string item;

    while ( std::getline(ss, item, delim) )
    {
        *(result++) = item;
    }

}

void Graph::addEdge(uint_fast32_t a, uint_fast32_t b, std::unordered_set<uint_fast32_t> frequencies)
{

  list[a][b] = frequencies;
  list[b][a] = frequencies;

  numEdges++;

}

void Graph::addFrequency(uint_fast32_t a, uint_fast32_t b, uint_fast32_t f)
{

  list[a][b].emplace(f);
  list[b][a].emplace(f);

}


bool doPathsHaveCollision(Path &path1, Path &path2)
{

  //o conjunto de arestas reúne um par (aresta, frequência)
  //uma aresta é um par de vértices
  std::unordered_set< std::pair < std::pair<uint_fast32_t, uint_fast32_t>, uint_fast32_t > > path1Edges, path2Edges;

  //recuperar as arestas dos dois caminhos
  for(uint_fast32_t i = 0; i < path1.nodeList.size(); i++)
  {

    if( i + 1 < path1.nodeList.size() )
    {
      std::pair<uint_fast32_t, uint_fast32_t> edge(path1.nodeList[i], path1.nodeList[i+1]);
      std::pair<uint_fast32_t, uint_fast32_t> reverseEdge(path1.nodeList[i+1], path1.nodeList[i]);
      uint_fast32_t frequency = path1.frequency;

      std::pair < std::pair<uint_fast32_t, uint_fast32_t>, uint_fast32_t > edgeWithFrequency(edge, frequency);
      std::pair < std::pair<uint_fast32_t, uint_fast32_t>, uint_fast32_t > reverseEdgeWithFrequency(reverseEdge, frequency);
      path1Edges.emplace(edgeWithFrequency);
      path1Edges.emplace(reverseEdgeWithFrequency);
    }

  }

  for(uint_fast32_t i = 0; i < path2.nodeList.size(); i++)
  {

    if( i + 1 < path2.nodeList.size() )
    {
      std::pair<uint_fast32_t, uint_fast32_t> edge(path2.nodeList[i], path2.nodeList[i+1]);
      std::pair<uint_fast32_t, uint_fast32_t> reverseEdge(path2.nodeList[i+1], path2.nodeList[i]);
      uint_fast32_t frequency = path2.frequency;

      std::pair < std::pair<uint_fast32_t, uint_fast32_t>, uint_fast32_t > edgeWithFrequency(edge, frequency);
      std::pair < std::pair<uint_fast32_t, uint_fast32_t>, uint_fast32_t > reverseEdgeWithFrequency(reverseEdge, frequency);
      path2Edges.emplace(edgeWithFrequency);
      path2Edges.emplace(reverseEdgeWithFrequency);
    }

  }

  //comparar as arestas dos dois caminhos
  for(auto it: path1Edges)
  //for(uint_fast32_t i = 0; i < path1Edges.size(); i++)
  {
    //encontrei uma aresta com mesmos vértices e mesma frequência
    if( path2Edges.find(*it) != path2Edges.end() ){
    //if(path2Edges.find(path1Edges[]) != path2Edges.end())
      return true;
    }
  }

  return false;


}
