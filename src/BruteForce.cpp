#include "BruteForce.hpp"
#include <algorithm> //std::max_element
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair

void BruteForce::run(Graph &graph, std::vector< std::pair<intType, intType> > & requestedConnections,
   std::vector< std::unordered_set<intType> > frequencies, intType frequencyIndex,
    std::vector< std::unordered_set<intType> > &bestSolution, intType &bestSolutionValue)
{
  //representação

  //número de arestas do grafo, também é o maximo de frequẽncias que podemos alocar
  intType edges = graph.numEdges;
  intType maxFrequencies = graph.numEdges + 1; //lembrar do grafo linha

  //caso inicial:
  //frequencies é um vetor de graph.numEdges x 0 elementos
  //frequencyIndex = 0
  //bestSolution é um vetor de graph.numEdges x 0 elementos
  //bestSolutionValue = graph.numEdges + 2, 1 a mais que o limite


  if ( frequencyIndex == maxFrequencies ) //já cheguei ao limite de frequências
  {

    if ( isViable(graph, requestedConnections, frequencies) )
    {

      //coletando número de frequências alocadas(maior elemento do vetor + 1)
      auto largestSetIterator = std::max_element(std::begin(frequencies),
                                     std::end(frequencies),
                                     [](const std::unordered_set<intType>& lhs,
                                        const std::unordered_set<intType>& rhs)
                                     {
                                       return lhs.size() < rhs.size();
                                     });

      intType numFrequencies = *largestVectorIterator.size();

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
    for (intType i = 0; i < edges; i++)
    {

      //não alocar essa frequência...
      algorithm(graph, requestedConnections, frequencies, frequencyIndex + 1, solutionValue, bestSolution);

      //ou alocar
      frequencies[i].emplace(frequencyIndex);
      algorithm(graph, requestedConnections, frequencies, frequencyIndex + 1, solutionValue, bestSolution);
    }

  }


}

bool BruteForce::isViable(Graph &graph, std::vector< std::pair<intType, intType> > &requestedConnections,
   std::vector< std::unordered_set<intType> > &frequencies)
{
  //viável significa que as frequências ligam os pares de vértices pedidos
  //e que não há dois caminhos que compartilham arestas com a mesma frequência
  //alocada

  //transfere as frequências do vetor para o grafo

  intType edgeIndex = 0;

  for (auto & vertex: graph.list)
  {

    for (auto & neighbor: vertex.second)
    //vizinhos e frequências
    {
      neighbor.second = frequencies[edgeIndex]
      edgeIndex++;
      //isso com certeza não passa do número de arestas
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
    bool foundAPath;

    //varre todos os vizinhos da origem
    for (auto & neighbor: graph.list[source])
    {

      presentNode = neighbor.first;

      for (auto & freq: neighbor.second) //unordered_set com frequências alocadas na aresta
      {

        presentFrequency = freq;
        presentPath.nodeList.clear();
        presentPath.nodeList.push_back(source);
        presentPath.nodeList.push_back(presentNode);

        if( findPath(graph, presentNode, destination, presentFrequency, presentPath) )
        {
          presentPath.frequency = presentFrequency;
          pathList.push_back(presentPath);
          foundAPath = true;
          break;
        }

      }

      if(foundAPath) break;

    }

    if(!foundAPath) return false;

    //para todos os caminhos, verifica se existem arestas compartilhadas entre eles
    //usando a mesma frequência para os dois caminhos

    for(intType firstIndex = 0; firstIndex < pathList.size(); firstIndex++)
    {

      for(intType secondIndex = firstIndex + 1; firstIndex < pathList.size(); firstIndex++)
      {
        if( doPathsHaveCollision( pathList[firstIndex], pathList[secondIndex] ) ) return false;
      }

    }

  }

  return true;

}

bool BruteForce::findPath(Graph &graph, intType source, intType destination, intType frequency, Path &presentPath)
{

  if (source == destination) return true;

  for (auto & neighbor: graph.list[source])
  {

    //se o iterador de fim for o retorno, não foi encontrado
    if ( neighbor.second.find(frequency) != neighbor.second.end() )
    {
        intType presentNode = neighbor.first;
        presentPath.nodeList.push_back(presentNode);
        return findPath(graph, presentNode, destination, frequency, presentPath);
    }

  }

  //retorno false se não consegui encontrar o destino
  return false;


}

bool BruteForce::doPathsHaveCollision(Path &path1, Path &path2)
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
