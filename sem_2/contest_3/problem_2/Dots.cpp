//
// Created by User on 024 24.04.18.
//

#include "Dots.h"

void generateDots(unsigned int seed, Graph &graph) {
  vector<pair<int, int>> dots;
  srand(seed);
  for (unsigned int i = 0; i < graph.GetVerticesNumber(); ++i) {
    // Преобразование Бокса — Мюллера
    double u = (rand() % 2001) / 1000.0 - 1.0;
    double v = sqrt(1 - u * u) * (rand() % 2001 / 1000.0 - 1.0);  // в диапозоне -1..1
    double s = u * u + v * v;
    double p = sqrt(-2 * log(s) / s);
    //  елси значение черезчур маленькое , то обнуляем его
    if (fabs(s - 1) < 0.0001)
      p = 0;
    double x = 100 * u * p;
    double y = 100 * v * p;
    dots.emplace_back(x, y);
    // добавляем ребра
    for (unsigned int j = 0; j < i; ++j) {
      graph.AddEdge(i, j, sqrt(pow(x - dots[j].first, 2.0)
                                   + pow(y - dots[j].second, 2.0)));
    }
  }
}