//
// Created by User on 017 17.02.18.
//

#ifndef SEM_2_1_CSETGRAPH_H
#define SEM_2_1_CSETGRAPH_H

#pragma once
#include <unordered_set>
#include "IGraph.h"

using std::unordered_set;

//хранящий граф в виде массива хэш-таблиц
class CSetGraph: public IGraph {
 public:
  explicit CSetGraph (unsigned int verticesNumber);
  explicit CSetGraph (const IGraph* graph);


  // Добавление ребра от from к to.
  void AddEdge(int from, int to)  ;

  int VerticesCount() const;

  void GetNextVertices(int vertex, std::vector<int> &vertices) const override;
  void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;

 private:

  unsigned int verticesNumber_;

  vector<unordered_set<int>> in_;
  vector<unordered_set<int>> out_;

};

#endif //SEM_2_1_CSETGRAPH_H