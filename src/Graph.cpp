#include "Graph.hpp"
#include <fstream> //std::ifstream
#include <iterator> //std::back_inserter
#include <string> //std::string, std::stoi

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

void Graph::addEdge(intType a, intType b, std::vector<intType> frequencies)
{

  list[a][b] = frequencies;
  list[b][a] = frequencies;

  numEdges++;

}

void Graph::addFrequency(intType a, intType b, intType f)
{

  list[a][b].push_back(f);
  list[b][a].push_back(f);

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

    std::vector<intType> emptyVector;
    addEdge(node1, node2, emptyVector);

  }

}
