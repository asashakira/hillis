#pragma once

#include <vector>

using namespace std;

class TestCases {
  float fitness;
  int testsize;
  int inputsize;
public:
  vector<vector<int>> testcases, t1, t2, g1, g2;
  TestCases(int n, int m, vector<vector<int>> &tests);
  TestCases(int n, int m);
  void Merge();
  void CreateGametes();
  void Crossover(TestCases &b);
  void Mutate(int mutation_rate);
  float Fitness() { return fitness; }
  void SetFitness(float f) { fitness = f; }
  int Size() { return testcases.size(); }
};
