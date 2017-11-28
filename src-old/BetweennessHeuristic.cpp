#include "BetweennessHeuristic.hpp"
#include "Graph.hpp"
#include <unordered_set>
#include <vector> //std::vector
#include <utility> //std::pair
#include <stack> //std::stack

int BetweennessHeuristic::getMinDistanceVertex(std::vector<int> vec)
{

  int minElement = UINT_FAST32_MAX;
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

  std::vector<int> dist(graph.size(), UINT_FAST32_MAX);
  dist[source] = 0;
  //distância infinita para todos menos a raiz

  //considerando que "infinito" é o mesmo que sem predecessor
  std::vector<int> pred(graph.size(), UINT_FAST32_MAX);

  //S_ tem todos os vértices
  std::unordered_set<int> S_;


  for(auto & it: graph.list)
  {
    S_.emplace(it.first);
  }

  while(!S.empty())
  {
    int u = getMinDistanceVertex(dist);

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

bool BetweennessHeuristic::calculate(Graph &graph, std::vector< std::pair<int, int> > & requestedConnections, int & value)
{

  //chave: aresta(vértice1, vértice2), valor mapeado: betweenness
  std::map< std::pair<int, int>, int > edges;
  bool isTherePath = true;
  int maxBetweenness = 0;

  for(auto & connection: requestedConnections)
  {

    Path path = dijkstraForBetweenness(graph, connection.first, connection.second);
    isTherePath = !path.nodeList.empty();

    if(!isTherePath) return false;

    else
    {

      for(int i = 1; i < path.nodeList.size(); i++)
      {
        std::pair<int, int> edge(path.nodeList[i - 1], path.nodeList[i]);
        std::pair<int, int> reverseEdge(path.nodeList[i], path.nodeList[i - 1]);
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

  value = maxBetweenness;
  return true;

}
