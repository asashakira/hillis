#include "Coevolution.h"
#include "Random.h"
#include <map>
#include <algorithm>
#include <math.h>
#include <thread>

using namespace std;

Coevolution::Coevolution(int popsize, int crossover, int mutation, int inputsize, int comparesize, int testsize) {
  population_size = popsize;
  height = width = sqrt(popsize);
  crossover_rate = crossover;
  mutation_rate = mutation;
  input_size = inputsize;
  compare_size = comparesize;
  test_size = testsize;

  // init host (sortingnetworks)
  host.resize(height);
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      host[i].push_back(SortingNetwork(comparesize, inputsize));

  // making testcases
  vector<pair<vector<int>,int>> tests(1 << input_size);
  for (int bit = 0; bit < (1 << input_size); bit++) {
    vector<int> s(input_size);
    int zero = 0;
    for (int i = 0; i < input_size; i++) {
      s[i] = bit & 1<<i ? 1 : 0;
      zero += s[i] == 0;
    }
    tests[bit] = {s, zero};
  }
  // init parasite
  parasite.resize(height);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      TestCases t(testsize, inputsize);
      for (int k = 0; k < test_size; k++) {
        int r = fastrng() % (int)tests.size();
        t.testcases[k].testcase = tests[r].first;
        t.testcases[k].zeros = tests[r].second;
      }
      parasite[i].push_back(t);
    }
  }
}

void Coevolution::CalculateFitness(SortingNetwork &sn, TestCases &tc) {
}

void Coevolution::Evaluate() {
  for (int i = 0; i < height; i++) for (int j = 0; j < width; j++)
    Coevolution::CalculateFitness(host[i][j], parasite[i][j]);
}

void Coevolution::Selection() {
  const int di[] = {1, 0, -1, 0};
  const int dj[] = {0, 1, 0, -1};

  // replace lower-scoring individual
  // host
  vector<pair<float,pair<int,int>>> v(population_size);
  for (int i = 0; i < height; i++) for (int j = 0; j < width; j++)
    v[i] = {host[i][j].Fitness(), {i, j}};
  sort(v.begin(), v.end());
  for (int idx = 0; idx < population_size/2; idx++) {
    auto [i, j] = v[idx].second;
    for (int d = 0; d < 4; d++) {
      int ni = (i+di[d]+height) % height;
      int nj = (j+dj[d]+width) % width;
      if (host[i][j].Fitness() < host[ni][nj].Fitness())
        host[i][j] = host[ni][nj];
    }
  }
  // parasite
  for (int i = 0; i < height; i++) for (int j = 0; j < width; j++)
    v[i] = {parasite[i][j].Fitness(), {i, j}};
  sort(v.begin(), v.end());
  for (int idx = 0; idx < population_size/2; idx++) {
    auto [i, j] = v[idx].second;
    for (int d = 0; d < 4; d++) {
      int ni = (i+di[d]+height) % height;
      int nj = (j+dj[d]+width) % width;
      if (parasite[i][j].Fitness() < parasite[ni][nj].Fitness())
        parasite[i][j] = parasite[ni][nj];
    }
  }

  for (int i = 0; i < height; i++) for (int j = 0; j < width; j++) {
    host[i][j].CreateGametes();
    parasite[i][j].CreateGametes();
  }

  // walk around grid to find mate
  // host
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int ni = i, nj = j;
      while (fastrng() % 2) {
        int r = fastrng() % 4;
        ni = (i+di[r]+height) % height;
        nj = (j+dj[r]+width) % width;
      }
      // crossover
      if (i == ni and j == nj) continue;
      host[i][j].Crossover(host[ni][nj]);
    }
  }
  // parasite
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int ni = i, nj = j;
      while (fastrng() % 2) {
        int r = fastrng() % 4;
        ni = (i+di[r]+height) % height;
        nj = (j+dj[r]+width) % width;
      }
      // crossover
      if (i == ni and j == nj) continue;
      parasite[i][j].Crossover(parasite[ni][nj]);
    }
  }

  // Mutation
  // host
  for (auto &networks : host)
    for (auto &network : networks)
      network.Mutate(mutation_rate);
  // parasite
  // TODO
}

float Coevolution::AverageFitness() {
  float sum = 0;
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      sum += host[i][j].Fitness();
  return sum / population_size;
}

SortingNetwork Coevolution::GetBestNetwork() {
  int best_i, best_j;
  best_i = best_j = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (host[best_i][best_j].Fitness() < host[i][j].Fitness()) {
        best_i = i;
        best_j = j;
      }
    }
  }
  return host[best_i][best_j];
}

void Coevolution::Print() {
  for (auto &p : host) {
    for (auto &x : p) {
      cout << x.Fitness() << '\n';
      x.Print();
      cout << '\n';
    }
  }
}
void Coevolution::PrintPopulation() {
  for (auto &p : host) {
    for (auto &x : p)
      cout << x.Fitness() << ' ';
    cout << '\n';
  }
}
