#include "Random.h"
#include "Coevolution.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <future>

using namespace std;

signed main() {
  // const int popsize = 65536; // must be rootable
  const int popsize = 25; // must be rootable
  const int crossover = popsize / 2;
  const int mutation = 1000;
  const int inputsize = 6;
  const int comparesize = 12;
  const int testsize = 10;
  const int max_generation = 500;

  auto start = chrono::high_resolution_clock::now();
  Coevolution co(popsize, crossover, mutation, inputsize, comparesize, testsize);
  co.Evaluate();
  for (int gen = 1; gen <= max_generation; gen++) {
    if (gen % 100 == 0) {
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
  // auto sn = co.GetBestNetwork();
  auto sn = co.AllTimeBest();
  sn.Print(); cout << '\n';
  cout << sn.Test() << "%\n";
  cout << "Size: " << sn.Size() << '\n';


  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(stop-start);
  cout << "Time: " << duration.count() << "ms\n";
}
