//
// Created by User on 017 17.02.18.
//

#ifndef SEM_2_1_CMATRIXGRAPH_H
#define SEM_2_1_CMATRIXGRAPH_H

#pragma once
#include "IGraph.h"

//хранящий граф в виде матрицы смежности
class CMatrixGraph: public IGraph{
 public:
  explicit CMatrixGraph(unsigned int verticesNumber);
  explicit CMatrixGraph(const IGraph* graph);


  // Добавление ребра от from к to.
  void AddEdge(int from, int to) ;

  int VerticesCount() const;

  void GetNextVertices(int vertex, std::vector<int> &vertices) const override;
  void GetPrevVertices(int vertex, std::vector<int> &vertices) const override;

 private:

  unsigned int verticesNumber_;

  vector<vector<bool>> edges_;

};

#endif //SEM_2_1_CMATRIXGRAPH_H
