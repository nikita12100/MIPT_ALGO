//
// Created by User on 017 17.02.18.
//

#ifndef SEM_2_1_CSETGRAPH_H
#define SEM_2_1_CSETGRAPH_H

#pragma once
#include <unordered_set>
#include <vector>

using std::unordered_set;
using std::vector;

//хранящий граф в виде массива хэш-таблиц
class CSetGraph{
 public:
  explicit CSetGraph (unsigned int verticesNumber);
  explicit CSetGraph (const CSetGraph* graph);


  // Добавление ребра от from к to.
  void AddEdge(int from, int to)  ;

  int VerticesCount() const;

  void GetNextVertices(int vertex, std::vector<int> &vertices) const;
  void GetPrevVertices(int vertex, std::vector<int> &vertices) const;

  bool Is_stock();

 private:

  unsigned int verticesNumber_;

  vector<unordered_set<int>> in_;
  vector<unordered_set<int>> out_;

};

#endif //SEM_2_1_CSETGRAPH_H