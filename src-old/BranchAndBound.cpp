#include "BranchAndBound.hpp"
#include <algorithm> //std::max_element
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair
#include <vector> //std::vector
#include <algorithm> //std::max, std::min
#include "BetweennessHeuristic.hpp"
#include "Graph.hpp"

//calcula a quantidade de frequências necessárias em um grafo linha
//onde todas as possíveis combinações de vértices devem ter uma conexão
//(pior caso)
void PathGlobalUpperLimit::calculate(Graph & graph)
{

  int numPathEdges = graph.list.size() - 1

  std::vector<int> pathEdges(numPathEdges, 0);

  for (int i = 0; i < numPathEdges; i++)
  {

    pathEdges[i] = (numPathEdges - i) * (i + 1);

  }

  int maxValue = 0;

  for (auto & it: pathEdges){ if(it > maxValue) maxValue = it; }

  value = maxValue;


}

void BranchAndBound::run(Graph &graph, std::vector< std::pair<int, int> > & requestedConnections,
   std::vector< std::unordered_set<int> > frequencies, int frequencyIndex,
   std::vector< std::pair<int, int> > connectionsToDo)
{


  //primeira iteração
  if(frequencyIndex == 0)
  {
    pathLimit.calculate(graph);

    BetweennessHeuristic btwn;
    int betweennessLimitValue;
    betweennessViable = btwn.calculate(graph, requestedConnections, betweennessLimitValue);
    if(!betweennessViable) return; //problema inviável (não existe caminho para alguma das conexões?)

    globalUpperLimit.value = std::min(pathLimit.value, betweennessLimitValue);
    globalUpperLimit.isViable = true;

    globalLowerLimit.value = 1; //melhor caso: grafo completo
    globalLowerLimit.isViable = true;

    maxFrequencies = globalUpperLimit.value;

    std::unordered_set<int> startingFrequency;
    startingFrequency.emplace(0);

    for(int i = 0; i < maxFrequencies; i++) frequencies.push_back(startingFrequency);

  }

  //coletando número de frequências alocadas(tamanho do maior vetor de frequências)
  auto largestFrequencyIterator = std::max_element(std::begin(frequencies),
                                 std::end(frequencies),
                                 [](const std::unordered_set<int>& lhs,
                                    const std::unordered_set<int>& rhs)
                                 {
                                   return lhs.size() < rhs.size();
                                 });

  int numFrequencies = *largestFrequencyIterator.size();


  //determinando o limite superior local

  //menor valor entre o limite global e o valor da solução atual + por quantas frequências falta iterar
  localUpperLimit.value = std::min(globalUpperLimit.value, numFrequencies + ( (maxFrequencies - 1) - frequencyIndex ) );

  localUpperLimit.isViable = true;

  localLowerLimit.value = std::max(1, numFrequencies);
  localLowerLimit.isViable = true;

  int numEdges = graph.numEdges;

  //poda por limitante
  if(localLowerLimit > globalUpperLimit) return;

  //verifica viabilidade
  checkConnections(graph. requestedConnections, frequencies, connectionsToDo);


//chegamos ao final das iterações ou ocorreu uma poda por otimalidade
  if (frequencyIndex == maxFrequencies or localUpperLimit.value == localLowerLimit.value)
  {


    //poda por viabilidade
    if( connectionsToDo.empty() ){

      //coletando número de frequências alocadas(maior elemento do vetor + 1)
      auto largestSetIterator = std::max_element(std::begin(frequencies),
                                     std::end(frequencies),
                                     [](const std::unordered_set<int>& lhs,
                                        const std::unordered_set<int>& rhs)
                                     {
                                       return lhs.size() < rhs.size();
                                     });

      int numFrequencies = *largestSetIterator.size();

      if (numFrequencies < bestSolutionValue)
      {
        bestSolutionValue = numFrequencies;
        bestSolution.clear();

        if(bestSolutionValue < globalUpperLimit) globalUpperLimit = bestSolutionValue;

        for (int i = 0; i < edges; i++)
        {
          bestSolution.push_back(frequencies[i]);
        }

      }

    }

    else return;

  }

  else
  {


    for (int i = 0; i < edges; i++)
    {

      //não alocar essa frequência...
      run(graph, requestedConnections, frequencies, frequencyIndex + 1, solutionValue, bestSolution);

      //...ou alocar
      frequencies[i].emplace(frequencyIndex);
      run(graph, requestedConnections, frequencies, frequencyIndex + 1, solutionValue, bestSolution);
    }

  }

}

Graph BranchAndBound::makeFrequencyGraph(Graph graph, std::vector< std::unordered_set<int> > &frequencies)
{

  int edgeIndex = 0;

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
bool BranchAndBound::checkConnection(Graph graph, std::pair<int, int> connection, Path & path)
{

  int source = connection.first;
  int destination = connection.second;

  int presentNode;
  int presentFrequency;
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


void BranchAndBound::checkConnections(Graph graph, std::vector< std::pair<int, int> > & requestedConnections,
   std::vector< std::unordered_set<int> > & frequencies, std::vector< std::pair<int, int> > &connectionsToDo)
{

  //verifica viabilidade:
  //1) todas as conexões pedidas foram feitas?
  //2) as conexões estão livres de colisões?

  //transfere as frequências do vetor para o grafo

  Graph g = makeFrequencyGraph(graph, frequencies);

  std::vector<Path> pathList;

  std::vector< std::pair<int, int> > updateToDo;

  //verifica se todos os pedidos de conexão foram "atendidos"
  for (auto & connection: requestedConnections)
  {
    Path path;

    if( checkConnection(g, connection, path) )
    {
      pathList.push_back(path);
    }

    else updateToDo.push_back(connection);
    //a conexão não foi atendida se não tem caminho

  }

  //para todos os caminhos, verifica se existem arestas compartilhadas entre eles
  //usando a mesma frequência para os dois caminhos

  for(int firstIndex = 0; firstIndex < pathList.size(); firstIndex++)
  {

    for(int secondIndex = firstIndex + 1; firstIndex < pathList.size(); secondIndex++)
    {

      if( doPathsHaveCollision( pathList[firstIndex], pathList[secondIndex] ) )
      {

        std::pair<int, int> firstConnection(*pathList[firstIndex].nodeList.begin(), *(pathList[firstIndex].nodeList.end() - 1) )
        std::pair<int, int> secondConnection(*pathList[secondIndex].nodeList.begin(), *(pathList[secondIndex].nodeList.end() - 1) )
        updateToDo.push_back(firstConnection);
        updateToDo.push_back(secondConnection);
        //nenhuma das conexões foi atendida se ambas colidem

      }

    }

  }

  connectionsToDo.clear();
  connectionsToDo = updateToDo;

}

//algoritmo recursivo para encontrar um caminho entre um vértice de origem e um vértice de destino
bool BranchAndBound::findPath(Graph &graph, int source, int destination, int frequency, Path &presentPath)
{

  if (source == destination) return true;
  bool foundAPath = false;

  for (auto & neighbor: graph.list[source])
  {

    //se o iterador de fim for o retorno, não foi encontrado
    if ( neighbor.second.find(frequency) != neighbor.second.end() )
    {
        int presentNode = neighbor.first;
        presentPath.nodeList.push_back(presentNode);
        foundAPath = findPath(graph, presentNode, destination, frequency, presentPath);
        if(foundAPath) break;
    }

  }

  //retorno false se não consegui encontrar o destino
  return foundAPath;


}
