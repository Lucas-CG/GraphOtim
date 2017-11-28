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

  int numPathEdges = graph.numVertices - 1

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
    int betweennessLimitValue = btwn.calculate(graph, requestedConnections);
    if(betweennessLimitValue == -1) return; //problema inviável (não existe caminho para alguma das conexões)

    globalUpperLimit.value = std::min(pathLimit.value, betweennessLimitValue);
    globalUpperLimit.isViable = true;

    globalLowerLimit.value = 1; //melhor caso: grafo completo
    globalLowerLimit.isViable = true;

    maxFrequencies = globalUpperLimit.value;

    std::unordered_set<int> startingFrequencies;
    startingFrequencies.emplace(0);

    //todas as arestas recebem 1 frequência (esse é o mínimo para conectar todos)
    for(int i = 0; i < graph.numEdges; i++) frequencies.push_back(startingFrequencies);

  }

  //coletando maior número de frequências alocadas(tamanho do maior vetor de frequências)
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
  //maxFrequencies - 1 porque as frequências são indexadas começando-se de 0
  localUpperLimit.value = std::min(globalUpperLimit.value, numFrequencies + ( (maxFrequencies - 1) - frequencyIndex ) );

  localUpperLimit.isViable = true;

  localLowerLimit.value = std::max(1, numFrequencies);
  localLowerLimit.isViable = true;

  //poda por limitante
  if(localLowerLimit > globalUpperLimit) return;

  //verifica viabilidade e quantas conexões ainda não foram feitas
  checkConnections(graph. requestedConnections, frequencies, connectionsToDo);


//chegamos ao final das iterações
  if (frequencyIndex == maxFrequencies)
  {

    //poda por inviabilidade -> só armazeno a solução se esse conjunto for vazio, ou seja, se for viável
    if( connectionsToDo.empty() )
    {

      if (numFrequencies < bestSolutionValue)
      {
        bestSolutionValue = numFrequencies;
        bestSolution.clear();

        if(bestSolutionValue < globalUpperLimit) globalUpperLimit = bestSolutionValue;

        for (auto & it: frequencies)
        {
          bestSolution.push_back(it);
        }

        return; //não preciso mais buscar; obtive uma solução viável

      }

    }

    else return; //não obtive uma solução viável e já se passaram todas as iterações. sair.

  }

  //poda por otimalidade -> aqui eu descarto viabilidade por ser uma solução intermediária
  else if(localUpperLimit.value == localLowerLimit.value)
  {

    if (localUpperLimit.value < bestSolutionValue)
    {
      bestSolutionValue = localUpperLimit.value;
      bestSolution.clear();

      if(bestSolutionValue < globalUpperLimit.value) globalUpperLimit.value = bestSolutionValue;

    }

    //encontrando ou não um valor melhor, parar a busca porque não vou encontrar
    //nada melhor nesse ramo
    return;

  }

  //não chegamos a nenhum critério de poda ou solução viável
  else
  {

    //aloco ou não a frequência atual em qualquer uma das arestas do grafo
    for (int i = 0; i < graph.numEdges; i++)
    {

      //não alocar essa frequência...
      run(graph, requestedConnections, frequencies, frequencyIndex + 1, connectionsToDo);

      //...ou alocar
      frequencies[i].emplace(frequencyIndex);
      run(graph, requestedConnections, frequencies, frequencyIndex + 1, connectionsToDo);
    }

  }

}

Graph BranchAndBound::makeFrequencyGraph(Graph graph, std::vector< std::unordered_set<int> > &frequencies)
{

  int edgeIndex = 0;

  for (int i = 0; i < graph.numVertices; i++)
  {

    for (int j = 0; i < graph.numVertices; j++)
    //vizinhos e frequências
    {

      //se existe aresta
      if(graph.matrix[i][j].first && i != j)
      {
        graph.matrix[i][j].second = frequencies[edgeIndex];
        edgeIndex++;
      }

      // já preenchi todas as arestas. o resto são repetições na matriz
      if(edgeIndex == graph.numEdges) break;
    }

    if(edgeIndex == graph.numEdges) break;

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
  for (int j = 0; j < graph.numVertices; j++)
  {

    if(graph.matrix[source][j].first) //se é vizinho...
    {

      presentNode = j;

      //varre por todas as frequências alocadas naquela aresta
      for (auto & freq: graph.matrix[source][j].second)
      {

        presentFrequency = freq;

        //inicia a formação de um novo caminho com a frequência freq
        presentPath.nodeList.clear();
        presentPath.nodeList.push_back(source);
        presentPath.nodeList.push_back(presentNode);

        //tenta encontrar um caminho até o destino a partir do vizinho com essa frequência
        if( findPath(graph, presentNode, destination, presentFrequency, presentPath) )
        {
          presentPath.frequency = presentFrequency;
          foundAPath = true;
          path = presentPath;
          return true;
        }

      }

    }

    //não achei nada; o caminho retornado é vazio
    if(!foundAPath)
    {
      path.clear();
      return false;
    }

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

  //verifica se todos os pedidos de conexão foram "atendidos" - ainda não estou considerando colisões
  for (auto & connection: requestedConnections)
  {
    Path path;

    if( checkConnection(g, connection, path) )
    {
      //achei um caminho pronto para a conexão, adiciono à lista para verificar se há colisõess
      pathList.push_back(path);
    }

    //a conexão não foi atendida se não existe um caminho
    else updateToDo.push_back(connection);


  }

  //para todos os caminhos da lista, verifica se existem arestas compartilhadas entre eles
  //usando a mesma frequência para os dois caminhos

  for(int firstIndex = 0; firstIndex < pathList.size(); firstIndex++)
  {

    for(int secondIndex = firstIndex + 1; firstIndex < pathList.size(); secondIndex++)
    {

      if( doPathsHaveFrequencyCollision( pathList[firstIndex], pathList[secondIndex] ) )
      {

        //recupera as conexões formadas pelos dois caminhos
        std::pair<int, int> firstConnection(pathList[firstIndex].nodeList[0], pathList[firstIndex].nodeList[pathList[firstIndex].nodeList.size() - 1] );;
        std::pair<int, int> secondConnection(pathList[secondIndex].nodeList[0], pathList[secondIndex].nodeList[pathList[secondIndex].nodeList.size() - 1] )


        //nenhuma das conexões foi atendida se ambas colidem
        updateToDo.push_back(firstConnection);
        updateToDo.push_back(secondConnection);

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

  //vizinhos da origem
  for (int j = 0; j < graph.numVertices; j++)
  {

    //se o iterador de fim for o resultado, não foi encontrado
    if (graph.matrix[source][j].first && graph.matrix[source][j].second.find(frequency) != graph.matrix[source][j].second.end() )
    {
        int presentNode = j;
        presentPath.nodeList.push_back(presentNode);
        foundAPath = findPath(graph, presentNode, destination, frequency, presentPath);
        if(foundAPath) break;
    }

  }

  //retorno false se não consegui encontrar o destino
  return foundAPath;


}
