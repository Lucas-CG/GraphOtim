#include "SplittingHeuristic.hpp"
#include <queue> //std::priority_queue
#include <stack> //std::stack
#include <utility> //std::pair
#include <vector> //std::vector
#include <climits> //INT_MAX

int SplittingHeuristic::getMinDistanceVertex(std::vector<int> vec)
{

  int minElement = INT_MAX;
  int minElementIndex;

  for(int i = 0; i < vec.size(); i++)
  {

    if(vec[i] < minElement)
    {
      minElement = vec[i];
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

  std::vector<int> dist(graph.numVertices, INT_MAX);
  dist[source] = 0;
  //distância infinita para todos menos a raiz

  //-1 = sem predecessor
  std::vector<int> pred(graph.numVertices, -1);

  //S_ tem todos os vértices
  std::unordered_set<int> S_;


  for(int i = 0; i < graph.numVertices; i++)
  {
    S_.emplace(i);
  }

  while(!S_.empty())
  {
    int u = getMinDistanceVertex(dist);

    S_.erase(u);

    if(u == destination){

      break;
      foundAPath = true;

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
    Path path = dijkstraForSplitting(graph, connection.first, connection.second);
    isTherePath = !path.nodeList.empty();

    if(!isTherePath){
      pathList.clear();
      break;
    }

    else
    {
      pathList.push_back(path);
    }


  }

  return pathList;

}


Graph SplittingHeuristic::generateCollisionGraph(Graph graph, std::vector<Path> pathList)
{

  Graph collisionGraph( pathList.size() );


  //para todos os caminhos, verifica se existem arestas compartilhadas entre eles
  //se existirem, é necessário separá-los alocando uma frequência para cada um
  //isso é representado com uma aresta entre dois nós, cada um representando um caminho,
  //sendo o grafo gerado usado em um algoritmo de coloração

  for(int firstIndex = 0; firstIndex < pathList.size(); firstIndex++)
  {

    for(int secondIndex = firstIndex + 1; firstIndex < pathList.size(); secondIndex++)
    {

      if( doPathsHaveCollision( pathList[firstIndex], pathList[secondIndex] ) )
      {

        collisionGraph.addEdge(firstIndex, secondIndex);

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

    pq.emplace(vertex);
  }

  std::pair<int, int> nextVertex;

  while( !pq.empty() ) {

    nextVertex = pq.top();
    pq.pop();

    if( colorArrangement.empty() )
    {
      std::vector<int> newColor;

      //nextVertex.first é o índice do vértice
      newColor.push_back(nextVertex.first);

      colorArrangement.push_back(newColor);
    }

    else
    {
        for (auto & color: colorArrangement)
        {
          for (int i = 0; i < color.size(); i++)
          {
            //se for vizinho de quem eu quero inserir, não posso incluir na mesma cor

            if(nextVertex.first == color[i]) break;

            if(i == color.size() - 1) color.push_back(nextVertex.first);
            else continue;
          }

        }

    }

  }

  numColors = colorArrangement.size();

  return numColors;

}

int SplittingHeuristic::calculate(Graph graph, std::vector< std::pair<int, int> > & requestedConnections){

  std::vector<Path> pathList = generateShortestPathsForRequestedConnections(graph, requestedConnections);

  if( pathList.empty() ) return -1;
  
  Graph collisionGraph = generateCollisionGraph(graph, pathList);
  return greedyColoring(collisionGraph);

}
