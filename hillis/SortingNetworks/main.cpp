#include "Random.h"
#include "GeneticAlgorithm.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <thread>
#include <chrono>
#include <future>

using namespace std;

void Test(SortingNetwork &sn, int n) {
  vector<int> a(n);
  iota(a.begin(), a.end(), 1);
  for (int i = 0; i < n; i++) {
    int r = fastrng() % n;
    swap(a[i], a[r]);
  }
  auto b = a;
  sort(b.begin(), b.end());
  sn.Sort(a);
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (a[i] == b[i]) count++;
    cout << b[i] << ' ' << a[i] << ' ' << (a[i] == b[i] ? "OK" : "NG") << '\n';
  }
  cout << (float)count / n * 100 << "%\n";
  cout << "Size: " << sn.Size() << '\n';
}

signed main() {
  // const int popsize = 65536; // must be rootable
  const int popsize = 10000; // must be rootable
  const int crossover = popsize / 2;
  const int mutation = 1000;
  const int inputsize = 16;
  const int comparesize = 60;
  const int testsize = 1000;
  const int max_generation = 100;

  GeneticAlgorithm ga(popsize, crossover, mutation, inputsize, comparesize, testsize);
  ga.Evaluate();
  for (int gen = 1; gen <= max_generation; gen++) {
    if (gen % 10 == 0) {
      cout << "gen " << gen << " : ";
      cout << ga.AverageFitness() << '\n';
      cout << ga.GetBestNetwork().Fitness() << '\n';
      // ga.PrintPopulation();
    }
    auto start = chrono::high_resolution_clock::now();
    ga.Selection();
    ga.Evaluate();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop-start);
    // cout << duration.count() << '\n';
  }
  // ga.PrintPopulation();
  auto sn = ga.GetBestNetwork();
  // sn.Print(); cout << '\n';
  Test(sn, inputsize);
  cout << sn.Fitness() << '\n';
}
