//
// Created by User on 024 24.04.18.
//

#ifndef TRAVELINGSSALESMAN_DOTS_H
#define TRAVELINGSSALESMAN_DOTS_H

#include "Graph.h"
#include <cmath>
#include <cstdlib>

using std::srand;
using std::rand;
using std::sqrt;
using std::log;
using std::move;

// верншины графа координаты точек , а вес - расстояние
// Гауссово распределение (дисперсия 1, мат. ожидание (0,0))
void generateDots(unsigned int seed, Graph &graph);

#endif //TRAVELINGSSALESMAN_DOTS_H
