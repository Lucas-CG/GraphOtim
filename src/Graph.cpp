#include "Graph.hpp"
#include <fstream> //std::ifstream
#include <iterator> //std::back_inserter
#include <string> //std::string, std::stoi
#include <unordered_set> //std::unordered_set

/*
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

*/

Graph::Graph(int size)
{

  numVertices = size;

  std::unordered_set<int> nullFreqs;

  std::pair<bool, std::unordered_set<int> > pair(false, nullFreqs);

  std::vector< std::pair< bool, std::unordered_set<int> > > pairVector(size, pair);

  std::vector< std::vector< std::pair< bool, std::unordered_set<int> > > > mat(size, pairVector);

  matrix = mat;

}

void Graph::addEdge(int a, int b)
{

  matrix[a][b].first = true;
  matrix[b][a].first = true;

  numEdges++;

}

void Graph::addFrequency(int a, int b, int f)
{

  matrix[a][b].second.emplace(f);
  matrix[b][a].second.emplace(f);

}


bool doPathsHaveCollision(Path &path1, Path &path2)
{

  //o conjunto de arestas reúne um par (aresta, frequência)
  //uma aresta é um par de vértices
  std::vector< std::pair<int, int> > path1Edges, path2Edges;

  //recuperar as arestas dos dois caminhos
  for(int i = 0; i < path1.nodeList.size(); i++)
  {

    if( i + 1 < path1.nodeList.size() )
    {
      std::pair<int, int> edge(path1.nodeList[i], path1.nodeList[i+1]);
      std::pair<int, int> reverseEdge(path1.nodeList[i+1], path1.nodeList[i]);

      path1Edges.push_back(edge);
      path1Edges.push_back(reverseEdge);
    }

  }

  for(int i = 0; i < path2.nodeList.size(); i++)
  {

    if( i + 1 < path2.nodeList.size() )
    {
      std::pair<int, int> edge(path2.nodeList[i], path2.nodeList[i+1]);
      std::pair<int, int> reverseEdge(path2.nodeList[i+1], path2.nodeList[i]);

      path2Edges.push_back(edge);
      path2Edges.push_back(reverseEdge);
    }

  }

  //comparar as arestas dos dois caminhos
  for(auto & it1: path1Edges)
  {

    for(auto & it2: path2Edges)
    {
      if(it1 == it2) return true;
    }

  }

  return false;


}

bool doPathsHaveFrequencyCollision(Path &path1, Path &path2)
{

  if(path1.frequency != path2.frequency) return false;

  else return doPathsHaveCollision(path1, path2);

}
