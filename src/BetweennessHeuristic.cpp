#include "BetweennessHeuristic.hpp"
#include "Graph.hpp"
#include <unordered_set>
#include <vector> //std::vector
#include <utility> //std::pair
#include <stack> //std::stack

int BetweennessHeuristic::getMinDistanceVertex(std::vector<int> vec)
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

Path BetweennessHeuristic::dijkstraForBetweenness(Graph graph, int source, int destination)
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


//retorna -1 se der erro (betweenness é sempre positiva)
int BetweennessHeuristic::calculate(Graph &graph, std::vector< std::pair<int, int> > & requestedConnections)
{

  //chave: aresta(vértice1, vértice2), valor mapeado: betweenness
  std::map< std::pair<int, int>, int > edges;
  bool isTherePath = true;
  int maxBetweenness = 0;

  for(auto & connection: requestedConnections)
  {

    Path path = dijkstraForBetweenness(graph, connection.first, connection.second);
    isTherePath = !path.nodeList.empty();

    if(!isTherePath) return -1;

    else
    {

      for(int i = 1; i < path.nodeList.size(); i++)
      {
        std::pair<int, int> edge(path.nodeList[i - 1], path.nodeList[i]);
        std::pair<int, int> reverseEdge(path.nodeList[i], path.nodeList[i - 1]);

        //soma 1 à betweenness da aresta no mapa;
        //se a aresta ainda não está no mapa, ela é inserida automaticamente
        edges[edge]++;
        edges[reverseEdge]++;
      }

    }

  }

  for(auto & edge: edges)
  {
    //edge.second = betweenness
    if(edge.second > maxBetweenness) maxBetweenness = it.second;
  }

  return maxBetweenness;

}
