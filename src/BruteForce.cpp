#include "BruteForce.hpp"
#include <algorithm> //std::max_element

void BruteForce::run(Graph &graph, std::vector< std::pair<intType, intType> > requestedConnections,
   std::vector< std::vector<intType> > frequencies, intType position,
    std::vector< std::vector<intType> > &bestSolution, intType &bestSolutionValue)
{
  //representação

  //número de arestas do grafo, também é o maximo de frequẽncias que podemos alocar
  intType edges = graph.numEdges;
  intType maxFrequencies = graph.numEdges + 1; //lembrar do grafo linha

  //caso inicial:
  //frequencies é um vetor de graph.numEdges x 0 elementos
  //position = 0
  //bestSolution é um vetor de graph.numEdges x 0 elementos
  //bestSolutionValue = graph.numEdges + 1


  if ( position == (maxFrequencies - 1) )
  {

    if ( isViable(graph, frequencies) )
    {

      //coletando número de frequências alocadas(maior elemento do vetor + 1)
      intType numFrequencies = 1 + *std::max_element( frequencies, std::begin(frequencies), std::end(frequencies) );

      if (numFrequencies < bestSolutionValue)
      {
        bestSolutionValue = numFrequencies;

        for (intType i = 0; i < edges; i++)
        {
          bestSolution[i] = frequencies[i];
        }

      }

    }

  }

  else
  {
    for (intType i = 0; i < maxFrequencies; i++)
    {
      algorithm(graph, requestedConnections, frequencies, position + 1, solutionValue, bestSolution)
      frequencies[position + 1] = i
      algorithm(graph, requestedConnections, frequencies, position + 1, solutionValue, bestSolution)
    }

  }


}

bool BruteForce::isViable(Graph &graph, std::vector< std::pair<intType, intType> > requestedConnections,
   std::vector<intType> frequencies)
{
  //viável significa que as frequências ligam os pares de vértices pedidos
  //e que não há dois caminhos que compartilham arestas com a mesma frequência
  //alocada

  //transfere as frequências do vetor para o grafo

  intType frequencyIndex = 0;

  for (auto & vertex: graph.list)
  {

    for (auto & neighbor: vertex.second)
    //vizinhos e frequências
    {
      neighbor.second = frequencies[frequencyIndex]
      frequencyIndex++;
    }

  }

  vector<Path> pathList;

  //verifica se todos os pedidos de conexão foram atendidos
  for (auto & connection: requestedConnections)
  {
    //realiza uma busca por profundidade pelo destino a partir da origem,
    //apenas com arestas de uma frequência, e constroi os caminhos formados
    intType source = connection.first;
    intType destination = connection.second;

    intType presentNode;
    intType presentFrequency;
    Path presentPath;

    //varre todos os vizinhos da origem
    for (auto & neighbor: graph.list[source])
    {

      presentNode = neighbor.first;
      presentFrequency = neighbor.second;
      presentPath.nodeList.clear();
      presentPath.nodeList.push_back(source);
      presentPath.nodeList.push_back(presentNode);

      if( findPath(graph, presentNode, destination, presentFrequency, presentPath) )
      {
        presentPath.frequency = presentFrequency;
        pathList.push_back(presentPath);
        break;
      }

    }

  }

}

bool BruteForce::findPath(Graph &graph, intType source, intType destination, intType frequency, Path &presentPath)
{

  if (source == destination) return true;

  for (auto & neighbor: graph.list[source])
  {

    if (neighbor.second == frequency)
    {
        intType presentNode = neighbor.first;
        presentPath.nodeList.push_back(presentNode);
        return findPath(graph, presentNode, destination, frequency, presentPath);
    }

  }

  //retorno false se não consegui encontrar o destino
  return false;


}
