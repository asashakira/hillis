#include "TestCases.h"
#include "Random.h"
#include <iostream>
#include <numeric>

TestCases::TestCases(int n, int m, vector<vector<int>> &tests) : t1(n), t2(n), g1(n), g2(n) {
  testsize = n;
  inputsize = m;
  for (int i = 0; i < n; i++)
    t1[i] = tests[fastrng() % (int)tests.size()];
  for (int i = 0; i < n; i++)
    t2[i] = tests[fastrng() % (int)tests.size()];
}

TestCases::TestCases(int n, int m) : t1(n), t2(n), g1(n), g2(n) {
  testsize = n;
  inputsize = m;
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
}

void TestCases::Crossover(TestCases &b) {
  t1 = g1;
  t2 = b.g2;
}

void TestCases::Mutate(int mutation_rate) {
  for (auto t : testcases) {
    for (int i = 0; i < inputsize; i++) {
      if (fastrng() % mutation_rate) continue;
      t[i] = fastrng() % inputsize;
      // t.testcase[i] ^= 1;
    }
  }
}
