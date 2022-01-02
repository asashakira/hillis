#include "GeneticAlgorithm.h"
#include "Random.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <thread>
#include <chrono>
using namespace std;

GeneticAlgorithm::GeneticAlgorithm(int popsize, int crossover, int mutation, int inputsize, int comparesize, int testsize) {
  population_size = popsize;
  height = width = sqrt(popsize);
  crossover_rate = crossover;
  mutation_rate = mutation;
  input_size = inputsize;
  compare_size = comparesize;
  test_size = testsize;
  // making population
  population.resize(height);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      population[i].push_back(SortingNetwork(comparesize, inputsize));
    }
  }
  // making test cases
  vector<pair<vector<int>,int>> v(1 << input_size);
  for (int bit = 0; bit < (1 << input_size); bit++) {
    vector<int> s(input_size);
    int zero = 0;
    for (int i = 0; i < input_size; i++) {
      s[i] = bit & 1<<i ? 1 : 0;
      zero += s[i] == 0;
    }
    v[bit] = {s, zero};
  }
  tests = v;
}

void GeneticAlgorithm::Evaluate() {
  vector<pair<vector<int>,int>> t(test_size); // tests to evaluate
  for (int i = 0; i < test_size; i++)
    t[i] = tests[fastrng() % (int)tests.size()];
  // evaluate fitness of each network
  vector<pair<float,pair<int,int>>> v(population_size);
  vector<thread> threads;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      threads.emplace_back(&SortingNetwork::EvaluateFitness, &population[i][j], ref(t));
      // population[i][j].EvaluateFitness(t);
    }
  }
  for (auto &t : threads) t.join();
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      v[i] = {population[i][j].Fitness(), {i, j}};
  sort(v.begin(), v.end());
  // cull bottom 50%
  for (int idx = 0; idx < population_size/2; idx++) {
    auto [i, j] = v[idx].second;
    auto [ni, nj] = v[population_size-1 - idx].second;
    population[i][j] = population[ni][nj];
  }
}

void GeneticAlgorithm::Selection() {
  const int di[] = {1, 0, -1, 0};
  const int dj[] = {0, 1, 0, -1};
  int d = fastrng() % 4;
  for (int i = 0; i < height; di[d] ? i+=2 : i++) {
    for (int j = 0; j < width; dj[d] ? j+=2 : j++) {
      int ni = (i+di[d]+height) % height;
      int nj = (j+dj[d]+width) % width;
      if (population[i][j].Fitness() < population[ni][nj].Fitness())
        swap(population[i][j], population[ni][nj]);
      population[ni][nj] = population[i][j];
    }
  }
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int ni = i, nj = j;
      int k = 1;
      while (true) {
        if (fastrng() % 2) break;
        d = fastrng() % 4;
        ni = (i+k*di[d]+k*height) % height;
        nj = (j+k*dj[d]+k*width) % width;
        k++;
      }
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

SortingNetwork GeneticAlgorithm::GetBestNetwork() {
  int best_i, best_j;
  best_i = best_j = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (population[best_i][best_j].Fitness() < population[i][j].Fitness()) {
        best_i = i;
        best_j = j;
      }
    }
  }
  return population[best_i][best_j];
}

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

