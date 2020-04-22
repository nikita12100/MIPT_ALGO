#include "TSP_LenPath.h"

double MinTourLenBF::operator()(const Graph &graph_) {
  graph = &graph_;
  verticesNumber = graph->GetVerticesNumber();
  tour.clear();

  if (verticesNumber <= 1)
    return 0;

  for (unsigned int i = 1; i < verticesNumber; ++i)
    tour.push_back(i);
  minTour = getTourLen();

  permutations(1);  // тут будут всевохможные перестановки и посчет их длины путей

  return minTour;
}

void MinTourLenBF::permutations(unsigned int depth) {
  // если переверули все вершины
  if (depth == tour.size()) {
    double len_tour = getTourLen();    // считаем длину пути
    if (len_tour < minTour)
      minTour = len_tour;   // елси нашли путь кароче
    return;
  }

  // рекурсивно получение перестановок
  for (int i = 0; i <= depth; i++) {
    // меняем местами от 0 до i все города
    unsigned int tmp = tour[i];
    tour[i] = tour[depth];
    tour[depth] = tmp;

    permutations(depth + 1);

    // возвращаем их обратно
    tmp = tour[i];
    tour[i] = tour[depth];
    tour[depth] = tmp;
  }
}

double MinTourLenBF::getTourLen() {
  double len_tour = graph->GetWeight(tour[verticesNumber - 2], 0)
      + graph->GetWeight(0, tour[0]);
  for (unsigned int i = 1; i < verticesNumber - 1; i++)
    len_tour += graph->GetWeight(tour[i - 1], tour[i]);
  return len_tour;
}
