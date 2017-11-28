#include "SplittingHeuristic.hpp"
#include <queue> //std::priority_queue
#include <stack> //std::stack
#include <utility> //std::pair
#include <vector> //std::vector
#include <climits> //INT_MAX
#include <iostream>
#include <unordered_set> //std::unordered_set

int SplittingHeuristic::getMinDistanceVertex(std::vector<int> dist, std::unordered_set<int> S_)
{

  int minElement = INT_MAX;
  int minElementIndex;

  for(int i = 0; i < dist.size(); i++)
  {

    //se a distância for menor e o vértice i estiver em S_
    if(dist[i] < minElement && S_.find(i) != S_.end())
    {
      minElement = dist[i];
      minElementIndex = i;
    }

  }

  //(vértice, distância)
  return minElementIndex;

}

Path SplittingHeuristic::dijkstraForSplitting(Graph graph, int source, int destination)
{

  //variáveis auxiliares
  Path shortestPath;
  bool foundAPath = false;

  ////std::cout << "criando vetor de distancias" << std::endl;
  std::vector<int> dist(graph.numVertices, INT_MAX);
  dist[source] = 0;
  //distância infinita para todos menos a raiz

  ////std::cout << "criando vetor de pred" << std::endl;
  //-1 = sem predecessor
  std::vector<int> pred(graph.numVertices, -1);

  //S_ tem todos os vértices

  ////std::cout << "criando S_" << std::endl;
  std::unordered_set<int> S_;


  for(int i = 0; i < graph.numVertices; i++)
  {
    S_.emplace(i);
    ////std::cout << "S_ <-" << i << std::endl;
  }

  while(!S_.empty())
  {

    //for(auto & it: dist) //std::cout << it << std::endl;

    //pega o vértice de menor distância que está em S_
    int u = getMinDistanceVertex(dist, S_);
    ////std::cout << "vértice " << u << " retirado de S_" << std::endl;

    S_.erase(u);

    if(u == destination)
    {

      ////std::cout << "destino encontrado" << std::endl;
      foundAPath = true;
      break;


    }

    //para todo vértice adjacente a u
    for (int v = 0; v < graph.numVertices; v++)
    {

      //se v é diferente de u, v é adjacente a u, v pertence a S_
      //e a distância de v é maior que a distância de u + 1 (aresta unitária)
      if(v != u && graph.matrix[u][v].first == true && S_.find(v) != S_.end() && dist[v] > dist[u] + 1)
      {
        dist[v] = dist[u] + 1;
        pred[v] = u;
      }

    }

  }

  if(foundAPath)
  {

    ////std::cout << "formando caminho mínimo" << std::endl;

    //obtendo o caminho da origem ao destino
    std::stack<int> stack; //a ordem é obtida com predecessores ao contrário; vamos inverter com uma pilha

    int next = destination;

    while(pred[next] != -1)
    {
      stack.push(next);
      next = pred[next];
    }

    stack.push(source);

    //recuperando da pilha
    while(!stack.empty())
    {
      shortestPath.nodeList.push_back( stack.top() );
      stack.pop();
    }

  }

  return shortestPath;

}

//gera os caminhos mínimos para todas as conexões pedidas
//retorna um vetor vazio se alguma conexão não puder ser feita
std::vector<Path> SplittingHeuristic::generateShortestPathsForRequestedConnections(Graph graph, std::vector< std::pair<int, int> > & requestedConnections)
{
  bool isTherePath = true;
  std::vector<Path> pathList;

  for(auto & connection: requestedConnections)
  {

    ////std::cout << "procurando caminho" << std::endl;
    Path path = dijkstraForSplitting(graph, connection.first, connection.second);
    isTherePath = !path.nodeList.empty();

    if(!isTherePath){
      pathList.clear();
      break;
    }

    else
    {
      ////std::cout << "caminho encontrando, adicionando" << std::endl;
      pathList.push_back(path);
    }


  }

  return pathList;

}


Graph SplittingHeuristic::generateCollisionGraph(Graph graph, std::vector<Path> pathList)
{

  //std::cout << "Inicializando grafo" << std::endl;
  Graph collisionGraph( pathList.size() );
  //std::cout << "Há " << pathList.size() << " caminhos." << std::endl;

  for(auto & it: pathList){

    for (auto & itt: it.nodeList){
      //std::cout << itt << ",";
    }

    //std::cout << std::endl;

  }

  //para todos os caminhos, verifica se existem arestas compartilhadas entre eles
  //se existirem, é necessário separá-los alocando uma frequência para cada um
  //isso é representado com uma aresta entre dois nós, cada um representando um caminho,
  //sendo o grafo gerado usado em um algoritmo de coloração

  for(int firstIndex = 0; firstIndex < pathList.size(); firstIndex++)
  {
    //std::cout << "firstIndex = " << firstIndex << std::endl;

    for(int secondIndex = firstIndex + 1; secondIndex < pathList.size(); secondIndex++)
    {

      //std::cout << "secondIndex = " << secondIndex << std::endl;

      if( doPathsHaveCollision( pathList[firstIndex], pathList[secondIndex] ) )
      {

        collisionGraph.addEdge(firstIndex, secondIndex);
        //std::cout << "aresta (" << firstIndex << "," << secondIndex << ")" << std::endl;

      }

    }

  }

  return collisionGraph;

}




int SplittingHeuristic::greedyColoring(Graph collisionGraph)
{

  //ideia:
  //priorizar pelo grau
  //retorno: número de cores

  int numColors;

  std::vector< std::vector<int> > colorArrangement;

  std::priority_queue< std::pair<int, int>, std::vector< std::pair<int, int> >, CompareDegrees > pq;

  //calcula o grau de todos os vértices e adiciona os pares (vértice, grau) à fila de prioridade

  for(int i = 0; i < collisionGraph.numVertices; i++)
  {

    int degree = 0;

    for(int j = 0; j < collisionGraph.numVertices; j++)
    {
      //existe aresta entre i e j?
      if(collisionGraph.matrix[i][j].first) degree++;
    }
    //(número do vértice, grau)
    std::pair<int, int> vertex(i, degree);
    //std::cout << "Inserindo vertice " << i << " com grau " << degree << std::endl;

    pq.emplace(vertex);
  }

  std::pair<int, int> nextVertex;

  while( !pq.empty() ) {

    bool foundColor = false;

    nextVertex = pq.top();
    pq.pop();

    //std::cout << "Retirado vértice " << nextVertex.first << " com grau" << nextVertex.second << std::endl;

    if( colorArrangement.empty() )
    {
      std::vector<int> newColor;

      //std::cout << "Primeira cor criada" << std::endl;

      //nextVertex.first é o índice do vértice
      newColor.push_back(nextVertex.first);

      //std::cout << "Vértice " << nextVertex.first << " inserido" << std::endl;

      colorArrangement.push_back(newColor);

      //std::cout << "Nova cor inserida na lista" << std::endl;

      foundColor = true;
    }

    else
    {
      for (auto & color: colorArrangement)
      {

        for (int i = 0; i < color.size(); i++)
        {
          //se for vizinho de quem eu quero inserir, não posso incluir na mesma cor
          if(collisionGraph.matrix[nextVertex.first][color[i]].first == true) break;

          //se cheguei ao final e não encontrei vizinhos
          if(i == color.size() - 1)
          {
            color.push_back(nextVertex.first);
            //std::cout << "Inserindo vértice " << nextVertex.first << " em cor existente" << std::endl;
            foundColor = true;
            break; //esse break está aqui pq a inserção aumenta o tamanho de 1, criando um loop infinito
          }
          else continue;
        }

        if(foundColor) break;

      }


      if(!foundColor)
      {
        //se eu terminei e não achei uma cor na qual eu possa inserir o vértice, crio uma nova
        std::vector<int> newColor;

        //std::cout << "Nova cor criada" << std::endl;

        //nextVertex.first é o índice do vértice
        newColor.push_back(nextVertex.first);

        //std::cout << "Vértice " << nextVertex.first << " inserido" << std::endl;

        colorArrangement.push_back(newColor);

        //std::cout << "Nova cor inserida na lista" << std::endl;

      }

    }

  }

  numColors = colorArrangement.size();

  return numColors;

}

int SplittingHeuristic::calculate(Graph graph, std::vector< std::pair<int, int> > & requestedConnections)
{

  //std::cout << "Gerando caminhos para as conexões" << std::endl;

  std::vector<Path> pathList = generateShortestPathsForRequestedConnections(graph, requestedConnections);

  if( pathList.empty() ) return -1;

  //std::cout << "Gerando grafo de colisão" << std::endl;

  Graph collisionGraph = generateCollisionGraph(graph, pathList);

  //std::cout << "Calculando coloração" << std::endl;

  return greedyColoring(collisionGraph);

}
