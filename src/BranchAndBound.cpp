#include "BranchAndBound.hpp"
#include <algorithm> //std::max_element
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair
#include <vector> //std::vector
#include <algorithm> //std::max

//calcula a quantidade de frequências necessárias em um grafo linha
//onde todas as possíveis combinações de vértices devem ter uma conexão
//(pior caso)
void PathGlobalUpperLimit::calculate(Graph & graph)
{

  intType numPathEdges = graph.list.size() - 1

  std::vector<intType> pathEdges(numPathEdges, 0);

  for (intType i = 0; i < numPathEdges; i++)
  {

    pathEdges[i] = (numPathEdges - i) * (i + 1);

  }

  intType maxValue = 0;

  for (auto & it: pathEdges){ if(it > maxValue) maxValue = it; }

  value = maxValue;


}

void BranchAndBound::run(Graph &graph, std::vector< std::pair<intType, intType> > & requestedConnections,
   std::vector< std::unordered_set<intType> > frequencies, intType frequencyIndex, bool hasPastFrequencyBeenAssigned,
   std::vector< std::pair<intType, intType> > connectionsToDo, std::vector< std::unordered_set<intType> > &bestSolution, intType &bestSolutionValue,
   LocalLowerLimit localLowerLimit, LocalUpperLimit localUpperLimit)
{

  if(frequencyIndex == 0)
  {

    pathLimit.calculate(graph);
    globalUpperLimit.value = pathLimit.value;
    globalUpperLimit.isViable = true;
    globalLowerLimit.value = 1; //melhor caso: grafo completo
    globalLowerLimit.isViable = true;
    maxFrequencies = pathLimit.value;

  }

  //coletando número de frequências alocadas(maior elemento do vetor + 1)
  auto largestFrequencyIterator = std::max_element(std::begin(frequencies),
                                 std::end(frequencies),
                                 [](const std::unordered_set<intType>& lhs,
                                    const std::unordered_set<intType>& rhs)
                                 {
                                   return lhs.size() < rhs.size();
                                 });

  intType numFrequencies = *largestFrequencyIterator.size();

  localUpperLimit.value = globalUpperLimit.value;
  localUpperLimit.isViable = globalUpperLimit.isViable;

  localLowerLimit.value = std::max(1, numFrequencies);
  localLowerLimit.isViable = true;

  intType edges = graph.numEdges;


  //caso inicial:
  //frequencies é um vetor de graph.numEdges arrays com apenas a frequência 0
  //frequencyIndex = 0
  //bestSolution é um vetor vazio
  //bestSolutionValue = infinito

  makeFrequencyGraph(graph, frequencies);

  for (auto it: connectionsToDo)
  {

    if (checkConnection(graph),)

  }


  if ( frequencyIndex == maxFrequencies or localUpperLimit.value == localLowerLimit.value or connectionsToDo.size() == 0) //cheguei ao limite de frequências ou os limites se encontraram
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

      intType numFrequencies = *largestSetIterator.size();

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
      //não-não-não-.. gera várias execuções com o mesmo estado. como cortar?
      run(graph, requestedConnections, frequencies, frequencyIndex + 1, solutionValue, bestSolution);

      //ou alocar
      frequencies[i].emplace(frequencyIndex);
      run(graph, requestedConnections, frequencies, frequencyIndex + 1, solutionValue, bestSolution);
    }

  }


}

Graph BranchAndBound::makeFrequencyGraph(Graph graph, std::vector< std::unordered_set<intType> > &frequencies)
{

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

  return graph;

}

//vê se uma conexão requisitada já foi atendida
bool BranchAndBound::checkConnection(Graph graph, std::pair<intType, intType> connection, Path & path)
{

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
        foundAPath = true;
        path = presentPath;
        return true;
      }

    }

  }

  if(!foundAPath)
  {
    path.clear();
    return false;
  }

}


bool BranchAndBound::isViable(Graph graph, std::vector< std::pair<intType, intType> > & requestedConnections,
   std::vector< std::unordered_set<intType> > & frequencies)
{
  //viável significa que as frequências ligam os pares de vértices pedidos
  //e que não há dois caminhos que compartilham arestas com a mesma frequência
  //alocada

  //transfere as frequências do vetor para o grafo

  Graph g = makeFrequencyGraph(graph, frequencies);

  vector<Path> pathList;

  //verifica se todos os pedidos de conexão foram atendidos
  for (auto & connection: requestedConnections)
  {
    Path path;

    if( checkConnection(g, connection, path) )
    {
      pathList.push_back(path);
    }

    else return false;

  }

  //para todos os caminhos, verifica se existem arestas compartilhadas entre eles
  //usando a mesma frequência para os dois caminhos

  for(intType firstIndex = 0; firstIndex < pathList.size(); firstIndex++)
  {

    for(intType secondIndex = firstIndex + 1; firstIndex < pathList.size(); secondIndex++)
    {
      if( doPathsHaveCollision( pathList[firstIndex], pathList[secondIndex] ) ) return false;
    }

  }

  return true;

}

//algoritmo recursivo para encontrar um caminho entre um vértice de origem e um vértice de destino
bool BranchAndBound::findPath(Graph &graph, intType source, intType destination, intType frequency, Path &presentPath)
{

  if (source == destination) return true;
  bool foundAPath = false;

  for (auto & neighbor: graph.list[source])
  {

    //se o iterador de fim for o retorno, não foi encontrado
    if ( neighbor.second.find(frequency) != neighbor.second.end() )
    {
        intType presentNode = neighbor.first;
        presentPath.nodeList.push_back(presentNode);
        foundAPath = findPath(graph, presentNode, destination, frequency, presentPath);
        if(foundAPath) break;
    }

  }

  //retorno false se não consegui encontrar o destino
  return foundAPath;


}

bool BranchAndBound::doPathsHaveCollision(Path &path1, Path &path2)
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
