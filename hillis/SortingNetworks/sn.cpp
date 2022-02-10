#include "GeneticAlgorithm.h"
#include "Random.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <future>
#include <math.h>

using namespace std;

void Test(SortingNetwork &sn, int n) {
  int count = 0;
  for (int bit = 0; bit < (1 << n); bit++) {
    vector<int> a(n);
    for (int i = 0; i < n; i++)
      a[i] = bit & 1<<i ? 1 : 0;
    auto b = a;
    sort(b.begin(), b.end());
    sn.Sort(a);
    if (a == b) count++;
  }
  cout << (float)count / (1<<n) * 100 << "%\n";
  cout << "Size: " << sn.Size() << '\n';
}

signed main() {
  // const int popsize = 65536; // must be rootable
  const int popsize = 625; // must be rootable
  const int crossover = popsize / 2;
  const int mutation = 1000;
  const int inputsize = 12;
  const int comparesize = 39;
  const int testsize = pow(2, inputsize) < 1000 ? pow(2, inputsize) : 1000;
  const int max_generation = 500;

  auto start = chrono::high_resolution_clock::now();
  GeneticAlgorithm ga(popsize, crossover, mutation, inputsize, comparesize, testsize);
  ga.Evaluate();
  for (int gen = 1; gen <= max_generation; gen++) {
    if (gen % 100 == 0) {
      cout << "gen: " << gen << '\n';
      cout << "avg: " << ga.AverageFitness() << '\n';
      auto sn = ga.GetBestNetwork();
      cout << sn.Fitness()*100 << "% " << sn.Size() << '\n';
      cout << '\n';
    }
    ga.Selection();
    ga.Evaluate();
  }
  // ga.PrintPopulation();
  auto sn = ga.GetBestNetwork();
  sn.Print(); cout << '\n';
  Test(sn, inputsize);
  // cout << sn.Fitness()*100 << "%\n";
  // cout << '\n';
  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(stop-start);
  cout << duration.count() << '\n';
}
