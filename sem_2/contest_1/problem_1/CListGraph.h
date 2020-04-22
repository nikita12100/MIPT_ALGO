#ifndef SEM_2_1_CLISTGRAPH_H
#define SEM_2_1_CLISTGRAPH_H

#pragma once
#include "IGraph.h"
#include <list>

using std::list;

//хранящий граф в виде массива списков смежности
class CListGraph:public IGraph {
 public:
  explicit CListGraph(unsigned int verticesNumber);
  explicit CListGraph(const IGraph* graph);

  void AddEdge(int from, int to);

  virtual int VerticesCount() const  ;

  void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
  void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;

 private:

  unsigned int verticesNumber_;

  vector<list<int>> in_;
  vector<list<int>> out_;

};


#endif //SEM_2_1_CLISTGRAPH_H
