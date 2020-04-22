#include "TSP_MST_MinPat.h"

double MinTourLenMST::operator()(const Graph &graph) {
  verticesNumber = graph.GetVerticesNumber();

  Graph mst = MST(graph);

  used.assign(verticesNumber, false);
  used[0] = true;
  DFS(mst, 0);    // нашли путь

  // подсчитаем его длину
  double len_tour = graph.GetWeight(tour[verticesNumber - 2], 0)
      + graph.GetWeight(0, tour[0]);
  for (unsigned i = 1; i < verticesNumber - 1; i++)
    len_tour += graph.GetWeight(tour[i - 1], tour[i]);

  return len_tour;
}

void MinTourLenMST::DFS(const Graph &mst, unsigned int dot) {
  auto related = mst.GetRelated(dot);
  for (auto ch : related) {
    if (!used[ch.first]) {
      tour.push_back(ch.first);
      used[ch.first] = true;
      DFS(mst, ch.first);
    }
  }
}