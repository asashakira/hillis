#include "TestCases.h"
#include "Random.h"
#include <iostream>
#include <numeric>

TestCases::TestCases(int n, int m, int mutation, vector<vector<int>> &tests) : testcases(2*n), t1(n), t2(n), g1(n), g2(n) {
  testsize = n;
  inputsize = m;
  mutation_rate = mutation;
  zeroone = true;
  for (int i = 0; i < n; i++)
    t1[i] = tests[fastrng() % (int)tests.size()];
  for (int i = 0; i < n; i++)
    t2[i] = tests[fastrng() % (int)tests.size()];
}

TestCases::TestCases(int n, int m, int mutation) : testcases(2*n), t1(n), t2(n), g1(n), g2(n) {
  testsize = n;
  inputsize = m;
  mutation_rate = mutation;
  zeroone = false;
  for (int i = 0; i < n; i++) {
    vector<int> a(m);
    iota(a.begin(), a.end(), 1);
    for (int j = 0; j < m; j++) {
      int r = fastrng() % m;
      swap(a[j], a[r]);
    }
    t1[i] = a;
  }
  for (int i = 0; i < n; i++) {
    vector<int> a(m);
    iota(a.begin(), a.end(), 1);
    for (int j = 0; j < m; j++) {
      int r = fastrng() % m;
      swap(a[j], a[r]);
    }
    t2[i] = a;
  }
}

void TestCases::Merge() {
  testcases.clear();
  for (int i = 0; i < (int)t1.size(); i++) {
    if (t1[i] == t2[i]) {
      testcases.push_back(t1[i]);
    } else {
      testcases.push_back(t1[i]);
      testcases.push_back(t2[i]);
    }
  }
}

void TestCases::CreateGametes() {
  g1 = t1;
  g2 = t2;
  int crossover_point = fastrng() % testsize;
  for (int i = crossover_point; i < testsize; i++)
    swap(g1[i], g2[i]);

  TestCases::Mutate();
}

void TestCases::Crossover(TestCases &b) {
  t1 = g1;
  t2 = b.g1;
}

void TestCases::Mutate() {
  for (int i = 0; i < testsize; i++) {
    for (int j = 0; j < inputsize; j++) {
      if (zeroone) {
        if (!(fastrng() % mutation_rate)) g1[i][j] ^= 1;
        if (!(fastrng() % mutation_rate)) g2[i][j] ^= 1;
      } else {
        if (!(fastrng() % mutation_rate)) swap(g1[i][j], g1[i][fastrng() % inputsize]);
        if (!(fastrng() % mutation_rate)) swap(g2[i][j], g2[i][fastrng() % inputsize]);
      }
    }
  }
}
