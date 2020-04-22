//
// Created by User on 024 24.04.18.
//

#include "Checker.h"

TestResults::TestResults(bool success_,
                         time_t time_,
                         double ansBF_,
                         double ansMST_,
                         double error_) :
    success(success_),
    time(time_),
    ansBF(ansBF_),
    ansMST(ansMST_),
    error(error_) {}

TestResults runTests(const vector<unsigned int> &seeds,
                     const vector<unsigned int> &ns,
                     double criteria) {
  unsigned int succ = 0;   //  количество удачных тестов
  unsigned int fail = 0;   //  кол-во неудачнх тестов
  time_t timer = 0;   //  общее время
  double avgMST = 0;    //  ответ МСТ
  double avgBF = 0;   //  точный ответ
  double stDeviation = 0;   //  стандартное отклонение (ошибки)

  for (unsigned int seed : seeds) {
    for (unsigned int n : ns) {
      TestResults results = runTest(seed, n, criteria);
      if (results.success)
        succ++;
      else
        fail++;
      timer += results.time;
      avgMST += results.ansMST;
      avgBF += results.ansBF;
      stDeviation += results.error;
    }
  }
  avgMST /= fail + succ;
  avgBF /= fail + succ;
  stDeviation = sqrt(stDeviation) / (fail + succ);

  cout << setw(42) << "============Testing result=============\n| "
       << setw(8) << succ + fail << setw(2) << " " << " Tests total" << setw(18) << " |\n| "
       << setw(8) << succ << setw(2) << " " << " Tests passed " << setw(16) << "|\n| "
       << setw(8) << fail << setw(2) << " " << " Tests failed " << setw(16) << "|\n| "
       << setw(8) << timer + 1 << setw(2) << " " << " Total time " << setw(18) << "|\n| "
       << setw(8) << std::fixed << setprecision(3)
       << (float) (timer + 1) / (succ + fail) << setw(2) << " " << " Avg time " << setw(20) << "|\n| "
       << setw(8) << avgBF << setw(2) << " " << " AVG BF answer " << setw(15) << "|\n| "
       << setw(8) << avgMST << setw(2) << " " << " AVG MST answer " << setw(14) <<  "|\n| "
       << setw(8) << stDeviation << setw(2) << " " << " Standard deviation " << setw(10) << "|\n| "
       << setw(8) << stDeviation / 100 << setw(30) << "Normalized st. deviation |\n";

  if (fail == 0)
    cout << setw(40) << "---------Testing  SUCCESSFUL----------\n";
  else
    cout << setw(40) << "------------Testing  FAILED------------\n";

  return (TestResults(fail == 0, timer, avgBF, avgMST, stDeviation));
}

TestResults runTests(const vector<unsigned int> &seeds,
                     unsigned int maxN,
                     double criteria) {
  vector<unsigned int> ns;
  for (unsigned int i = 2; i <= maxN; ++i)
    ns.push_back(i);
  return runTests(seeds, ns, criteria);
}

TestResults runTests(unsigned int maxSeed,
                     const vector<unsigned int> &ns,
                     double criteria) {
  vector<unsigned int> seeds;
  for (unsigned int i = 0; i <= maxSeed; ++i)
    seeds.push_back(i);
  return runTests(seeds, ns, criteria);
}

TestResults runTest(unsigned int seed, unsigned int n, double criteria) {
  Graph graph(n);
  generateDots(seed, graph);

  time_t tbegin = time(NULL);
  time_t timer = 0;
  MinTourLenBF minTourLenBF;
  MinTourLenMST minTourLenMST;
  double bf = minTourLenBF(graph);
  double mst = minTourLenMST(graph);
  timer = time(NULL) - tbegin;

  if (1 > (mst / bf) && (mst / bf) > criteria)
    cout << "!_FAIL_!";  //  не может быть больше 1 , так как это крачаешее решение
  double err = fabs(mst - bf) / bf * 100;
  cout << std::fixed << setprecision(2)
       << "BF_(" << bf << ") <-> MST_(" << mst << ")\n"
       << "{ " << (int) ((100 - err >= 0) ? (100 - err) : (0)) << "% }"
       << "TIME: " << timer
       << ", ERR: " << fabs(mst - bf)
      << "  [SEED=" << seed
      << ", N=" << n << "]"<< "\n\n";

  return (TestResults(1 <= mst / bf && mst / bf <= criteria,
                          timer, bf, mst, fabs(mst - bf)));
}