#include "SplittingHeuristic.hpp"
#include <map> //std::map
#include <queue> //std::priority_queue
#include <stack> //std::stack
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

  std::vector<int> dist(graph.list.size(), INT_MAX);
  dist[source] = 0;
  //distância infinita para todos menos a raiz

  //considerando que "infinito" é o mesmo que sem predecessor
  std::vector<int> pred(graph.list.size(), INT_MAX);

  //S_ tem todos os vértices
  std::unordered_set<int> S_;


  for(auto & it: graph.list)
  {
    S_.emplace(it.first);
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
    for (int v = 0; v < graph.list[u].size(); v++) //.first retorna o vizinho
    {

      //se v pertence a S_ e a distância de v é maior que a distância de u + 1 (aresta unitária)
      if(S_.find(v) != S_.end() && dist[v] > dist[u] + 1)
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

    while(pred[next] != UINT_FAST32_MAX)
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

  Graph collisionGraph;


  //para todos os caminhos, verifica se existem arestas compartilhadas entre eles
  //usando a mesma frequência para os dois caminhos

  for(int firstIndex = 0; firstIndex < pathList.size(); firstIndex++)
  {

    for(int secondIndex = firstIndex + 1; firstIndex < pathList.size(); secondIndex++)
    {

      if( doPathsHaveCollision( pathList[firstIndex], pathList[secondIndex] ) )
      {

        std::unordered_set<int> dummy;
        collisionGraph.addEdge(firstIndex, secondIndex, dummy);

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

  for(int i = 0; i < collisionGraph.list.size(); i++)
  {
    //(número do vértice, grau)
    std::pair<int, int> vertex( i, collisionGraph.list[i].size() );
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
    }

    else
    {
        for (auto & color: colorArrangement)
        {
          for (int i = 0; i < color.size(); i++)
          {
            //se for vizinho de quem eu quero inserir, não posso incluir na mesma cor
            if(collisionGraph.list[nextVertex.first].count(color[i]) == 0)
            {
              if(i == color.size() - 1) color.push_back(nextVertex.first);
              else continue;
            }

            else break;

          }

        }

    }

  }

  numColors = colorArrangement.size();

  return numColors;

}

int SplittingHeuristic::calculate(Graph graph, std::vector< std::pair<int, int> > & requestedConnections){

  std::vector<Path> pathList = generateShortestPathsForRequestedConnections(graph, requestedConnections);
  Graph collisionGraph = generateCollisionGraph(graph, pathList);
  return greedyColoring(collisionGraph);

}
