//
// Created by User on 013 13.02.18.
//

#ifndef SEM_2_1_IGRAPH_H
#define SEM_2_1_IGRAPH_H

#include <vector>

using std::vector;

struct IGraph {

  virtual ~IGraph() {}

  // Добавление ребра от from к to.
  virtual void AddEdge(int from, int to) = 0;

  virtual int VerticesCount() const = 0;

  virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0;
  virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0;

};

#endif //SEM_2_1_IGRAPH_H