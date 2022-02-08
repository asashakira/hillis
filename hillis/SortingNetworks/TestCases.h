#pragma once

#include <vector>

using namespace std;

class TestCases {
  float fitness;
  int testsize;
  int inputsize;
  int mutation_rate;
  bool zeroone;
public:
  vector<vector<int>> testcases, t1, t2, g1, g2;
  TestCases(int n, int m, int mutate, vector<vector<int>> &tests);
  TestCases(int n, int m, int mutate);
  void Merge();
  void CreateGametes();
  void Crossover(TestCases &b);
  void Mutate();
  float Fitness() { return fitness; }
  void SetFitness(float f) { fitness = f; }
  int Size() { return testcases.size(); }
};
