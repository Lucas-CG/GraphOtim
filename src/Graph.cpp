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

void Graph::addEdge(intType a, intType b, std::unordered_set<intType> frequencies)
{

  list[a][b] = frequencies;
  list[b][a] = frequencies;

  numEdges++;

}

void Graph::addFrequency(intType a, intType b, intType f)
{

  list[a][b].emplace(f);
  list[b][a].emplace(f);

}

void Graph::readFromFile(std::string fileName)
{

  std::ifstream inFile(fileName);
  string line;

  while ( std::getline(inFile, line) )
  {

    //cada linha corresponde a uma aresta
    vector<string> nodes;
    split( str, ' ', std::back_inserter(nodes) );

    intType node1 = (intType)std::stoi(nodes[0]);
    intType node2 = (intType)std::stoi(nodes[0]);

    std::unordered_set<intType> emptySet;
    addEdge(node1, node2, emptySet);

  }

}

bool doPathsHaveCollision(Path &path1, Path &path2)
{

  //o conjunto de arestas reúne um par (aresta, frequência)
  //uma aresta é um par de vértices
  std::unordered_set< std::pair < std::pair<intType, intType>, intType > > path1Edges, path2Edges;

  //recuperar as arestas dos dois caminhos
  for(intType i = 0; i < path1.nodeList.size(); i++)
  {

    if( i + 1 < path1.nodeList.size() )
    {
      std::pair<intType, intType> edge(path1.nodeList[i], path1.nodeList[i+1]);
      std::pair<intType, intType> reverseEdge(path1.nodeList[i+1], path1.nodeList[i]);
      intType frequency = path1.frequency;

      std::pair < std::pair<intType, intType>, intType > edgeWithFrequency(edge, frequency);
      std::pair < std::pair<intType, intType>, intType > reverseEdgeWithFrequency(reverseEdge, frequency);
      path1Edges.emplace(edgeWithFrequency);
      path1Edges.emplace(reverseEdgeWithFrequency);
    }

  }

  for(intType i = 0; i < path2.nodeList.size(); i++)
  {

    if( i + 1 < path2.nodeList.size() )
    {
      std::pair<intType, intType> edge(path2.nodeList[i], path2.nodeList[i+1]);
      std::pair<intType, intType> reverseEdge(path2.nodeList[i+1], path2.nodeList[i]);
      intType frequency = path2.frequency;

      std::pair < std::pair<intType, intType>, intType > edgeWithFrequency(edge, frequency);
      std::pair < std::pair<intType, intType>, intType > reverseEdgeWithFrequency(reverseEdge, frequency);
      path2Edges.emplace(edgeWithFrequency);
      path2Edges.emplace(reverseEdgeWithFrequency);
    }

  }

  //comparar as arestas dos dois caminhos
  for(auto & it: path1Edges)
  {
    //encontrei uma aresta com mesmos vértices e mesma frequência
    if(path2Edges.find(it) != path2Edges.end()){
      return true;
    }
  }

  return false;


}
