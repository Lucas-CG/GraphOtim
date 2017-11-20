#include "Graph.hpp"
#include <fstream> //std::ifstream
#include <iterator> //std::back_inserter
#include <string> //std::string, std::stoi

template<typename Out>
void split(const std::string &s, char delim, Out result) {

    std::stringstream ss;
    ss.str(s);
    std::string item;

    while ( std::getline(ss, item, delim) ) {
        *(result++) = item;
    }

}

void Graph::addEdge(nodeIndex a, nodeIndex b, frequencyIndex f){

  list[a][b] = f;
  list[b][a] = f;

}

void Graph::changeFrequency(nodeIndex a, nodeIndex b, frequencyIndex f) {

  addEdge(a, b, f);

}

void Graph::readFromFile(std::string fileName){

  std::ifstream inFile(fileName);
  string line;

  while ( std::getline(inFile, line) ){

    //cada linha corresponde a uma aresta
    vector<string> nodes;
    split( str, ' ', std::back_inserter(nodes) );

    nodeIndex node1 = (nodeIndex)std::stoi(nodes[0]);
    nodeIndex node2 = (nodeIndex)std::stoi(nodes[0]);

    addEdge(node1, node2, 0);

  }

}
