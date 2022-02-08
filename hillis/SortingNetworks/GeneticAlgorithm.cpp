#include "GeneticAlgorithm.h"
#include "Random.h"
#include <map>
#include <algorithm>
#include <math.h>
#include <thread>
#include <iostream>

using namespace std;

GeneticAlgorithm::GeneticAlgorithm(int popsize, int crossover, int mutation, int inputsize, int comparesize, int testsize) {
  population_size = popsize;
  height = width = sqrt(popsize);
  crossover_rate = crossover;
  mutation_rate = mutation;
  input_size = inputsize;
  compare_size = comparesize;
  test_size = testsize;
  SortingNetwork sn(comparesize, inputsize);
  best = sn;
  // making population
  population.resize(height);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      population[i].push_back(SortingNetwork(comparesize, inputsize));
    }
  }
  // making test cases
  vector<vector<int>> v(1 << input_size);
  for (int bit = 0; bit < (1 << input_size); bit++) {
    vector<int> s(input_size);
    for (int i = 0; i < input_size; i++)
      s[i] = bit & 1<<i ? 1 : 0;
    v[bit] = s;
  }
  tests = v;
}

const int di[] = {1, 0, -1, 0};
const int dj[] = {0, 1, 0, -1};

void GeneticAlgorithm::CalculateFitness(SortingNetwork &sn, vector<vector<int>> &tests) {
  float fitness = 0;
  sn.Merge();
  for (int i = 0; i < test_size; i++) {
    auto t = tests[i];
    auto a = t;
    sort(a.begin(), a.end());
    sn.Sort(t);
    if (t == a) fitness++;
  }
  fitness /= test_size;
  if (fitness == 1) fitness += 0.01 * (compare_size*2 - sn.Size());
  sn.SetFitness(fitness);
  if (sn.Fitness() >= best.Fitness()) best = sn;
}

void GeneticAlgorithm::Evaluate() {
  vector<vector<int>> t(test_size); // tests to evaluate
  for (int i = 0; i < test_size; i++)
    t[i] = tests[i];
    // t[i] = tests[fastrng() % (int)tests.size()];

  // evaluate fitness of each network
  vector<thread> threads;
  for (int i = 0; i < height; i++) for (int j = 0; j < width; j++)
    GeneticAlgorithm::CalculateFitness(population[i][j], t);
      // threads.emplace_back(&GeneticAlgorithm::Calculate_Fitness, &population[i][j], ref(t));
  // for (auto &t : threads) t.join();

}

void GeneticAlgorithm::Selection() {
  // cull bottom 50%
  vector<pair<float,pair<int,int>>> v(population_size);
  for (int i = 0; i < height; i++) for (int j = 0; j < width; j++)
    v[i] = {population[i][j].Fitness(), {i, j}};
  sort(v.begin(), v.end());
  for (int idx = 0; idx < population_size/2; idx++) {
    auto [i, j] = v[idx].second;
    for (int d = 0; d < 4; d++) {
      int ni = (i+di[d]+height) % height;
      int nj = (j+dj[d]+width) % width;
      if (population[i][j].Fitness() < population[ni][nj].Fitness())
        population[i][j] = population[ni][nj];
    }
  }

  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      population[i][j].CreateGametes();

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (i == v.back().second.first and j == v.back().second.second) continue;
      int ni = i;
      int nj = j;
      while (fastrng() % 2) {
        int r = fastrng() % 4;
        ni = (ni+di[r] + height) % height;
        nj = (nj+dj[r] + width) % width;
      }
      if (i == ni and j == nj) continue;
      population[i][j].Crossover(population[ni][nj]);
    }
  }

  for (auto &networks : population)
    for (auto &network : networks)
      network.Mutate(mutation_rate);
}

float GeneticAlgorithm::AverageFitness() {
  float sum = 0;
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      sum += population[i][j].Fitness();
  return sum / population_size;
}

SortingNetwork GeneticAlgorithm::GetBestNetwork() { return best; }

void GeneticAlgorithm::Print() {
  for (auto &p : population) {
    for (auto &x : p) {
      cout << x.Fitness() << '\n';
      x.Print();
      cout << '\n';
    }
  }
}
void GeneticAlgorithm::PrintPopulation() {
  for (auto &p : population) {
    for (auto &x : p)
      cout << x.Fitness() << ' ';
    cout << '\n';
  }
}
