
//  Travelling salesman problem
//  Надодит длину пути комивояжра
//  используя MST

#ifndef TRAVELINGSSALESMAN_TSP_LENPATH_H
#define TRAVELINGSSALESMAN_TSP_LENPATH_H

#include "Graph.h"

//  находим точную длину самого короткого пути (тупым перебором)
class MinTourLenBF {
 public:
  double operator()(const Graph &graph_);

 private:
  void permutations(unsigned int depth);  //  рассматриваем всевозможные пути

  double getTourLen();  // возвразает текущюю длину пути (так как список гордов измением)

  const Graph *graph; //  исходный граф
  unsigned int verticesNumber;
  vector<unsigned int> tour;    // список гродов на обход (его будем менять)
  double minTour;    //  длина минимального пути
};

#endif //TRAVELINGSSALESMAN_TSP_LENPATH_H
