#include "BranchAndBound.hpp"
#include <algorithm> //std::max_element
#include <unordered_set> //std::unordered_set
#include <utility> //std::pair
#include <vector> //std::vector
#include <algorithm> //std::max, std::min
#include "BetweennessHeuristic.hpp"
#include "Graph.hpp"
#include <climits> //INT_MAX
#include <iostream>

//calcula a quantidade de frequências necessárias em um grafo linha
//onde todas as possíveis combinações de vértices devem ter uma conexão
//(pior caso)
void PathGlobalUpperLimit::calculate(Graph & graph)
{

  std::cout << "Calculando pior caso" << std::endl;

  int numPathEdges = graph.numVertices - 1;

  std::vector<int> pathEdges(numPathEdges, 0);

  for (int i = 0; i < numPathEdges; i++)
  {

    std::cout << "pior caso: aresta " << i << std::endl;
    pathEdges.push_back( (numPathEdges - i) * (i + 1) );

  }

  int maxValue = 0;

  for (auto & it: pathEdges){ if(it > maxValue) maxValue = it; }

  value = maxValue;


}

void BranchAndBound::run(Graph &graph, std::vector< std::pair<int, int> > & requestedConnections,
   std::vector< std::unordered_set<int> > frequencies, int frequencyIndex,
   std::vector< std::pair<int, int> > connectionsToDo, LocalUpperLimit localUpperLimit, LocalLowerLimit localLowerLimit)
{

  std::cout << "imprimindo grafo" << std::endl;

  graph.print();

  //primeira iteração
  if(frequencyIndex == 0)
  {

    std::cout << "Primeira iteração" << std::endl;

    pathLimit.calculate(graph);


    std::cout << "calculando heurística de betweenness" << std::endl;
    BetweennessHeuristic btwn;
    int betweennessLimitValue = btwn.calculate(graph, requestedConnections);
    if(betweennessLimitValue == -1) return; //problema inviável (não existe caminho para alguma das conexões)

    std::cout << "valor pelo btwn: " << betweennessLimitValue << std::endl;

    globalUpperLimit.value = std::min(pathLimit.value, betweennessLimitValue);

    std::cout << "Limite superior global: " << globalUpperLimit.value << std::endl;

    globalUpperLimit.isViable = true;

    globalLowerLimit.value = 1; //melhor caso: grafo completo
    globalLowerLimit.isViable = true;

    maxFrequencies = globalUpperLimit.value;

    std::cout << "num max de freqs: " << maxFrequencies << std::endl;

    std::unordered_set<int> startingFrequencies;
    startingFrequencies.emplace(0);

    std::cout << "inserindo a frequência 0 em todas as arestas" << std::endl;

    //todas as arestas recebem 1 frequência (esse é o mínimo para conectar todos)
    for(int i = 0; i < graph.numEdges; i++) frequencies.push_back(startingFrequencies);

  }

  std::cout << "calculando o número de frequências alocadas até agora" << std::endl;
  //coletando maior número de frequências alocadas em uma aresta qualquer (tamanho do maior vetor de frequências)
  int numFrequencies = 0;

  for(auto & edge: frequencies)
  {
    if(edge.size() > numFrequencies) numFrequencies = edge.size();
  }

  std::cout << "e são... " << numFrequencies << std::endl;

  std::cout << "calculando limites locais" << std::endl;

  localUpperLimit.isViable = true;

  localLowerLimit.value = std::max(1, numFrequencies);

  std::cout << "limite inferior local: " << localLowerLimit.value << std::endl;

  localLowerLimit.isViable = true;

  //poda por limitante
  if(localLowerLimit.value > globalUpperLimit.value)
  {
    std::cout << "poda por limitante: " << localLowerLimit.value << " > " << globalUpperLimit.value << std::endl;
    return;
  }


  std::cout << "checando se todas as conexões foram feitas (viabilidade)" << std::endl;
  //verifica viabilidade e quantas conexões ainda não foram feitas
  checkConnections(graph, requestedConnections, frequencies, connectionsToDo);


  //determinando o limite superior local

  //menor valor entre o limite global e a soma entre quantas frequências já aloquei e quantas conexões ainda preciso fazer
  //maxFrequencies - 1 porque as frequências são indexadas começando-se de 0
  int missingPathsLimitValue = numFrequencies + connectionsToDo.size();
  localUpperLimit.value = std::min(globalUpperLimit.value, missingPathsLimitValue);

  std::cout << "limite superior do que falta: " << missingPathsLimitValue << std::endl;


  std::cout << "limite superior local: " << localUpperLimit.value << std::endl;


//chegamos ao final das iterações
  if (frequencyIndex == maxFrequencies)
  {
    std::cout << "última iteração" << std::endl;

    //poda por inviabilidade -> só armazeno a solução se esse conjunto for vazio, ou seja, se for viável
    if( connectionsToDo.empty() )
    {

      std::cout << "todas as conexões foram feitas (solução viável encontrada)" << std::endl;

      if (numFrequencies < bestSolutionValue)
      {
        std::cout << "encontrada solução melhor que atual! " << numFrequencies << " < " << bestSolutionValue << std::endl;
        bestSolutionValue = numFrequencies;
        bestSolution.clear();

        if(bestSolutionValue < globalUpperLimit.value)
        {
          globalUpperLimit.value = bestSolutionValue;
          std::cout << "melhor solução substitui limite! " << bestSolutionValue << " < " << globalUpperLimit.value << std::endl;
        }


        std::cout << "montando melhor solução" << std::endl;
        for (auto & it: frequencies)
        {
          bestSolution.push_back(it);
        }


        std::cout << "busca nesse nó encerrada" << std::endl;
        return; //não preciso mais buscar; obtive uma solução viável

      }

    }


    else{
      std::cout << "sem soluções viáveis após maxFrequencies iterações." << std::endl;
       return; //não obtive uma solução viável e já se passaram todas as iterações. sair.
    }
  }

  //poda por otimalidade -> aqui eu descarto viabilidade por ser uma solução intermediária
  else if(localUpperLimit.value == localLowerLimit.value)
  {
    std::cout << "limites se encontraram - poda por otimalidade" << std::endl;

    if (localUpperLimit.value < bestSolutionValue)
    {
      std::cout << "encontrada solução melhor que atual! " << localUpperLimit.value << " < " << bestSolutionValue << std::endl;
      bestSolutionValue = localUpperLimit.value;
      bestSolution.clear();

      if(bestSolutionValue < globalUpperLimit.value)
      {
        globalUpperLimit.value = bestSolutionValue;
        std::cout << "melhor solução substitui limite! " << bestSolutionValue << " < " << globalUpperLimit.value << std::endl;
      }

    }

    //encontrando ou não um valor melhor, parar a busca porque não vou encontrar
    //nada melhor nesse ramo
    std::cout << "busca encerrada devido à poda por otimalidade" << std::endl;
    return;

  }

  //não chegamos a nenhum critério de poda ou solução viável
  else
  {

    std::cout << "nenhum critério de poda satisfeito ou solução viável encontrada" << std::endl;

    //aloco ou não a frequência atual em qualquer uma das arestas do grafo
    for (int i = 0; i < graph.numEdges; i++)
    {

      std::cout << "criando novo filho: frequência " << frequencyIndex << " não alocada na aresta " << i << std::endl;
      //não alocar essa frequência...
      run(graph, requestedConnections, frequencies, frequencyIndex + 1, connectionsToDo, localUpperLimit, localLowerLimit);

      //...ou alocar
      std::cout << "criando novo filho: frequência " << frequencyIndex << " alocada na aresta " << i << std::endl;
      frequencies[i].emplace(frequencyIndex);
      run(graph, requestedConnections, frequencies, frequencyIndex + 1, connectionsToDo, localUpperLimit, localLowerLimit);
    }

  }

}

Graph BranchAndBound::makeFrequencyGraph(Graph graph, std::vector< std::unordered_set<int> > &frequencies)
{

  std::cout << "criando grafo com frequências" << std::endl;
  int edgeIndex = 0;

  std::cout << "numVertices = " << graph.numVertices << std::endl;

  for (int i = 0; i < graph.numVertices; i++)
  {
    std::cout << "graph.matrix[i], i = " << i << std::endl;

    //se j não for maior que i, vão ter arestas repetidas ( por ex.: (1,3) e (3, 1) )
    for (int j = i + 1; j < graph.numVertices; j++)
    //vizinhos e frequências
    {

      std::cout << "graph.matrix[i][j], j = " << j << std::endl;

      //se existe aresta
      if(graph.matrix[i][j].first && i != j)
      {
        std::cout << "existe aresta entre " << i << " e " << j << std::endl;
        std::cout << "alocando frequências na aresta (" << i << ", " << j << ")" << std::endl;

        graph.matrix[i][j].second.clear();
        graph.matrix[j][i].second.clear();

        graph.matrix[i][j].second = frequencies[edgeIndex];
        graph.matrix[j][i].second = frequencies[edgeIndex];


        edgeIndex++;
      }

      // já preenchi todas as arestas. o resto são repetições na matriz
      if(edgeIndex == graph.numEdges)
      {
        std::cout << "todas as arestas preenchidas" << std::endl;
        break;
      }
    }

    if(edgeIndex == graph.numEdges) break;

  }

  return graph;

}

//vê se uma conexão requisitada já foi atendida
bool BranchAndBound::checkConnection(Graph & graph, std::pair<int, int> connection, Path & path)
{

  std::cout << "checkConnection" << std::endl;

  int source = connection.first;
  int destination = connection.second;

  int presentNode;
  int presentFrequency;
  Path presentPath;
  bool foundAPath;

  //varre todos os vizinhos da origem
  for (int j = 0; j < graph.numVertices; j++)
  {

    std::cout << "varrendo vértice " << j << std::endl;

    std::cout << graph.matrix[source][j].first << std::endl;

    if(graph.matrix[source][j].first) //se é vizinho...
    {

      std::cout << j << " é vizinho de " << source << std::endl;

      presentNode = j;

      //varre por todas as frequências alocadas naquela aresta
      for (auto & freq: graph.matrix[source][j].second)
      {

        presentFrequency = freq;

        std::cout << "tomei uma frequência: " << freq << std::endl;

        //inicia a formação de um novo caminho com a frequência freq
        presentPath.nodeList.clear();
        presentPath.nodeList.push_back(source);
        presentPath.nodeList.push_back(presentNode);

        std::cout << "começando um novo caminho com frequência " << freq << " e nós " << source << " e " << presentNode << std::endl;

        //tenta encontrar um caminho até o destino a partir do vizinho com essa frequência
        if( findPath(graph, presentNode, destination, presentFrequency, presentPath) )
        {
          std::cout << "encontrei caminho!!!11ondeonzeee" << std::endl;
          presentPath.frequency = presentFrequency;
          foundAPath = true;
          path = presentPath;
          return true;
        }

      }

    }

  }

  //não achei nada; o caminho retornado é vazio
  if(!foundAPath)
  {
    std::cout << "não achei nada" << std::endl;
    path.nodeList.clear();
    return false;
  }

}

void BranchAndBound::checkConnections(Graph & graph, std::vector< std::pair<int, int> > & requestedConnections,
   std::vector< std::unordered_set<int> > & frequencies, std::vector< std::pair<int, int> > &connectionsToDo)
{

  //verifica viabilidade:
  //1) todas as conexões pedidas foram feitas?
  //2) as conexões estão livres de colisões?

  //transfere as frequências do vetor para o grafo
  Graph g = makeFrequencyGraph(graph, frequencies);

  std::cout << "imprimindo grafo com frequências" << std::endl;
  g.print();

  std::vector<Path> pathList;

  std::vector< std::pair<int, int> > updateToDo;

  std::cout << "verificando pedidos de conexão" << std::endl;
  //verifica se todos os pedidos de conexão foram "atendidos" - ainda não estou considerando colisões
  for (auto & connection: requestedConnections)
  {

    std::cout << "Pedido de conexão (" << connection.first << ", " << connection.second << ")" << std::endl;

    Path path;

    if( checkConnection(g, connection, path) )
    {
      std::cout << "caminho encontrado! adicionando à lista" << std::endl;
      //achei um caminho pronto para a conexão, adiciono à lista para verificar se há colisõess
      pathList.push_back(path);
    }

    //a conexão não foi atendida se não existe um caminho
    else{
      std::cout << "inserindo conexão no TODO - ainda não há caminho" << std::endl;
      updateToDo.push_back(connection);
    }


  }

  //para todos os caminhos da lista, verifica se existem arestas compartilhadas entre eles
  //usando a mesma frequência para os dois caminhos

  for(int firstIndex = 0; firstIndex < pathList.size(); firstIndex++)
  {

    for(int secondIndex = firstIndex + 1; secondIndex < pathList.size(); secondIndex++)
    {

      std::cout << "Caminho " << firstIndex << "X Caminho " << secondIndex << std::endl;

      if( doPathsHaveFrequencyCollision( pathList[firstIndex], pathList[secondIndex] ) )
      {

        std::cout << "tem colisão! pondo as duas conexões no TODO" << std::endl;

        //recupera as conexões formadas pelos dois caminhos
        std::pair<int, int> firstConnection(pathList[firstIndex].nodeList[0], pathList[firstIndex].nodeList[pathList[firstIndex].nodeList.size() - 1] );
        std::pair<int, int> secondConnection(pathList[secondIndex].nodeList[0], pathList[secondIndex].nodeList[pathList[secondIndex].nodeList.size() - 1] );


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
bool BranchAndBound::findPath(Graph & graph, int source, int destination, int frequency, Path &presentPath)
{

  std::cout << "findPath iniciando, porran" << std::endl;
  std::cout << "source: " << source << std::endl;
  std::cout << "destination: " << destination << std::endl;

  if (source == destination)
  {
    std::cout << "findPath: achei o destino" << std::endl;
    return true;
  }

  bool foundAPath = false;

  //vizinhos da origem
  for (int j = 0; j < graph.numVertices; j++)
  {

    std::cout << "findPath: olhando vértice " << j << std::endl;

    std::cout << "será que j já está no caminho?" << std::endl;

    bool isJInThePath = false;

    for(auto & vertex: presentPath.nodeList)
    {
        if(vertex == j) isJInThePath = true;
    }


    std::cout << "graph.matrix[" << source << "][" << j << "].first = " << graph.matrix[source][j].first << std::endl;
    std::cout << "is " << j << " in the path? " << isJInThePath << std::endl;
    std::cout << "achei frequência? " << (graph.matrix[source][j].second.find(frequency) != graph.matrix[source][j].second.end()) << std::endl;

    //se o iterador de fim for o resultado, não foi encontrado
    if (graph.matrix[source][j].first && !isJInThePath && graph.matrix[source][j].second.find(frequency) != graph.matrix[source][j].second.end() )
    {
        std::cout << "é vizinho, tem a frequência e não está no caminho; adicionando ao caminho" << std::endl;
        int presentNode = j;
        presentPath.nodeList.push_back(presentNode);
        foundAPath = findPath(graph, presentNode, destination, frequency, presentPath);
        if(foundAPath) break;
    }

  }

  //retorno false se não consegui encontrar o destino
  return foundAPath;


}
