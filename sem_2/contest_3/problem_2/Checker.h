//
// Created by User on 024 24.04.18.
//

#ifndef TRAVELINGSSALESMAN_CHECKER_H
#define TRAVELINGSSALESMAN_CHECKER_H

#include <iomanip>
#include <cmath>
#include <ctime>
#include "Graph.h"
#include "Dots.h"
#include "TSP_LenPath.h"
#include "TSP_MST_MinPat.h"

using std::cin;
using std::cout;
using std::fixed; //  вывод чисел с плавающей точкой по умолчанию
using std::setw;  //  ширина поля
using std::setprecision;  //  количество знаков после запятой
using std::move;

struct TestResults {
  TestResults(bool success,
              time_t time,
              double ansBF, //  TSPLenPath
              double ansMST,
              double error);

  bool success;   // исход теста
  time_t time;  // время тестирования в секундах
  double ansBF;   // TSPLenPath(Перебор,Brute force)
  double ansMST;  //TSPLenSMT
  double error;   //  отклонение
};

// запускаеим тесты от каджого зерна и от каждого номера точки
// criteria максимальне отношение ответа MST к ответу BF
TestResults runTests(const vector<unsigned int> &seeds,
                     const vector<unsigned int> &ns,
                     double criteria);

TestResults runTests(const vector<unsigned int> &seeds,
                     unsigned int maxN,
                     double criteria = 2);

TestResults runTests(unsigned int maxSeed,
                     const vector<unsigned int> &ns,
                     double criteria = 2);

// конкрестное зерно и номер вершины
TestResults runTest(unsigned int seed, unsigned int n, double criteria);

#endif //TRAVELINGSSALESMAN_CHECKER_H
