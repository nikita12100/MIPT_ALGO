#ifndef TRAVELINGSSALESMAN_TSP_MST_MINPAT_H
#define TRAVELINGSSALESMAN_TSP_MST_MINPAT_H

#include "Graph.h"
#include "MST.h"

class MinTourLenMST {
 public:
  double operator()(const Graph &graph);   // приближенное решение задачи

 private:
  void DFS(const Graph &mst, unsigned int dot);    /// Эйлеров путь в графе

  vector<unsigned int> tour;   // найденный путь в DFS
  vector<bool> used;    // поещенные в DFS
  unsigned int verticesNumber;
};

#endif //TRAVELINGSSALESMAN_TSP_MST_MINPAT_H
