#pragma once

#include <vector>

using namespace std;

class TestCase {
public:
  vector<int> testcase;
  int zeros;
};

class TestCases {
  float fitness;
public:
  vector<TestCase> testcases, g1, g2;
  TestCases(int testsize, int inputsize);
  float Fitness() { return fitness; }
  void CreateGametes();
  void Crossover(TestCases &b);
  void Mutate(int mutation_rate);
};
