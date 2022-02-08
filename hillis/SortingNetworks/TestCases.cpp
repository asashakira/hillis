#include "TestCases.h"

TestCases::TestCases(int testsize, int inputsize) : testcases(testsize) { }

void TestCases::CreateGametes() {
  g1 = c1;
  g2 = c2;
  for (int i = 0; i < compare_size; i+=4) {
    int crossover_point = fastrng() % 4;
    for (int j = i; j < i+crossover_point and j < compare_size; j++)
      swap(g1[i], g2[i]);
  }
}

void TestCases::Crossover(TestCases &b) {
  c1 = g1;
  c2 = b.g2;
  b.c2 = g2;
}

void TestCases::Mutate(int mutation_rate) {
}
