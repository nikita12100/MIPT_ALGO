#include "Checker.h"

int main() {
  vector<unsigned int> seeds{42};
  vector<unsigned int> quantities{10};

  cout << "Results for 10 points, 8 seeds:\n";
  runTests(7, quantities);

  cout << "\nResults for 2..12 points, 1 seed:\n";
  runTests(seeds, 12);

  return 0;
}