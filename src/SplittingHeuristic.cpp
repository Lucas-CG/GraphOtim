#include "SplittingHeuristic.hpp"
#include <unordered_map> //std::unordered_map
#include <queue> //std::priority_queue

intType SplittingHeuristic::getMinDistanceVertex(std::vector<intType> vec)
{

  intType minElement = INT_TYPE_MAX;
  intType minElementIndex;

  for(intType i = 0; i < vec.size(); i++)
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

Path SplittingHeuristic::dijkstraForSplitting(Graph graph, intType source, intType destination)
{

  //variáveis auxiliares
  Path shortestPath;
  bool foundAPath = false;

  std::vector<intType> dist(graph.size(), INT_TYPE_MAX);
  dist[source] = 0;
  //distância infinita para todos menos a raiz

  //considerando que "infinito" é o mesmo que sem predecessor
  std::vector<intType> pred(graph.size(), INT_TYPE_MAX);

  //S_ tem todos os vértices
  std::unordered_set<intType> S_;


  for(auto & it: graph.list)
  {
    S_.emplace(it.first);
  }

  while(!S.empty())
  {
    intType u = getMinDistanceVertex(dist);

    S_.erase(u);

    if(u == destination){

      break;
      foundAPath = true;

    }

    //para todo vértice adjacente a u
    for (auto & it: graph[u]) //.first retorna o vizinho
    {

      //se v pertence a S_ e a distância de v é maior que a distância de u + 1 (aresta unitária)
      if(S_.find(*it.first) != S_.end() && dist[*it.first] > dist[u] + 1)
      {
        dist[*it.first] = dist[u] + 1;
        pred[v] = u;
      }

    }

  }

  if(foundAPath)
  {
    //obtendo o caminho da origem ao destino
    std::stack stack; //a ordem é obtida com predecessores ao contrário; vamos inverter com uma pilha

    intType next = destination;

    while(pred[next] != INT_TYPE_MAX)
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

std::vector<Path> generateShortestPathsForRequestedConnections(Graph graph, std::vector< std::pair<intType, intType> > & requestedConnections)
{
  bool isTherePath = true;
  std::vector<Path> pathList;

  for(auto & connection: requestedConnections)
  {
    Path path = dijkstraForBetweenness(graph, connection.first, connection.second);
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

  for(intType firstIndex = 0; firstIndex < pathList.size(); firstIndex++)
  {

    for(intType secondIndex = firstIndex + 1; firstIndex < pathList.size(); secondIndex++)
    {

      if( doPathsHaveCollision( pathList[firstIndex], pathList[secondIndex] ) )
      {

        std::unordered_set<intType> dummy;
        collisionGraph.addEdge(firstIndex, secondIndex, dummy);

      }

    }

  }

  return collisionGraph;

}




intType SplittingHeuristic::greedyColoring(Graph collisionGraph)
{

  //ideia:
  //priorizar pelo grau
  //retorno: número de cores

  intType numColors;

  std::vector< std::vector<intType> > colorArrangement;

  std::priority_queue< std::pair<intType, intType>, std::vector< pair<intType, intType> >, CompareDegrees > pq;

  for(intType i = 0; i < collisionGraph.list.size(); i++)
  {
    //(número do vértice, grau)
    std::pair<intType, intType> vertex( i, collisiongraph.list[i].size() );
    pq.emplace(vertex);
  }

  while( !pq.empty() ) {

    nextVertex = pq.top();
    pq.pop();

    if( colorArrangement.empty() )
    {
      std::vector<intType> newColor;

      //nextVertex.first é o índice do vértice
      newColor.push_back(nextVertex.first);
    }

    else
    {
        for (auto & color: colorArrangement)
        {
          for (intType i = 0; i < color.size(), i++)
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

intType SplittingHeuristic::calculate(Graph graph, std::vector< std::pair<intType, intType> > & requestedConnections){

  std::vector<Path> pathList = generateShortestPathsForRequestedConnections(graph, requestedConnections);
  Graph collisionGraph = generateCollisionGraph(graph, pathList);
  return greedyColoring(collisionGraph);

}
