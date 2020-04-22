#include <iostream>
#include "CListGraph.h"
#include "CArcGraph.h"
#include "CSetGraph.h"
#include "CMatrixGraph.h"


int main(){
  auto SetGraph = new CSetGraph(5);

  int x,y = 0;
  for (int i = 0; i < 2; ++i) {
    std::cin >> x >> y;
    SetGraph->AddEdge(x,y);
  }

  auto ListGraph = new CListGraph(SetGraph);
  auto ArcGraph = new CArcGraph(SetGraph);
  auto MatrixGraph = new CMatrixGraph(SetGraph);

  delete(ListGraph);
  delete(ArcGraph);
  delete(MatrixGraph);
  delete(SetGraph);

  return 0;
}
