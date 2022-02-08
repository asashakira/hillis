#include "Random.h"
#include "Coevolution.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <future>

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
  const int popsize = 65536; // must be rootable
  // const int popsize = 2500; // must be rootable
  const int crossover = popsize / 2;
  const int mutation = 1000;
  const int inputsize = 8;
  const int comparesize = 16;
  const int testsize = 20;
  const int max_generation = 500;

  Coevolution co(popsize, crossover, mutation, inputsize, comparesize, testsize);
  co.Evaluate();
  for (int gen = 1; gen <= max_generation; gen++) {
    if (gen % 10 == 0) {
      cout << "gen: " << gen << '\n';
      cout << "host: " << co.AverageHostFitness() << '\n';
      cout << "para: " << co.AverageParasiteFitness() << '\n';
      auto sn = co.GetBestNetwork();
      cout << sn.Fitness()*100 << "%\n";
      cout << "Size: " << sn.Size() << '\n';
      cout << '\n';
    }
    co.Selection();
    co.Evaluate();
  }
  // co.PrintHost();
  // cout << '\n';
  // co.PrintParasite();
  auto sn = co.GetBestNetwork();
  sn.Print(); cout << '\n';
  Test(sn, inputsize);
}
