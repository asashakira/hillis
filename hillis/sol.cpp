
/*
hillis.cpp
Akira Itai
g++ -std=gnu++17 hillis.cpp -o a
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <random>

using namespace std;

// rng [from, to)
template<typename T>
T random(T from, T to) {
  random_device rand_dev;
  mt19937 g(rand_dev());
  uniform_real_distribution<double> d(from, to);
  return d(g);
}

template<typename T>
void print2d(vector<vector<T>> v) {
  for (int i = 0; i < (int)v.size(); i++) {
    for (int j = 0; j < (int)v[i].size(); j++) {
      cout << fixed << setprecision(5) << v[i][j] << ' ';
    }
    cout << '\n';
  }
}

class SortingNetwork {
private:
  vector<pair<int,int>> c1, c2, compares;
  float fitness = 0;
  int input_size;

  void Merge() {
    compares.clear();
    for (int i = 0; i < (int)c1.size(); i++) {
      if (c1[i].first == c2[i].first and c1[i].second == c2[i].second) {
        compares.push_back(c1[i]);
      } else {
        compares.push_back(c1[i]);
        compares.push_back(c2[i]);
      }
    }
  }
public:
  SortingNetwork() { }
  SortingNetwork(int size, int inputsize) : c1(size), c2(size) {
    input_size = inputsize;
    // create random network
    // c1
    for (auto &[a, b] : c1) {
      do {
        a = random(0, inputsize);
        b = random(0, inputsize);
      } while (a == b); // re-roll until a != b
      if (a > b) swap(a, b); // a must be less than b
    }
    // c2
    for (auto &[a, b] : c2) {
      do {
        a = random(0, inputsize);
        b = random(0, inputsize);
      } while (a == b); // re-roll until a != b
      if (a > b) swap(a, b); // a must be less than b
    }
  }
  SortingNetwork(vector<pair<int,int>> const &c) { compares = c; }

  void EvaluateFitness() {
    fitness = 0;
    map<vector<int>,int> mp; // to not test duplicates
    int max_tests = 10;
    int tests = 0;
    while (tests < max_tests) {
      vector<int> v(input_size);
      int zeros = 0;
      for (auto &x : v) {
        x = random(0, 2);
        zeros += x == 0;
      }
      if (mp.find(v) != mp.end()) continue;
      tests++;
      Sort(v);
      // calculating fitness
      for (auto x : v) {
        if (zeros) {
          fitness += x == 0 ? 1 : 0;
          zeros--;
        } else {
          fitness += x == 1 ? 1 : 0;
        }
      }
    }
  }

  void Cross(SortingNetwork &b, int cut) {
    for (int i = 0; i < cut; i++) {
      swap(c1[i], b.c1[i]);
      swap(c2[i], b.c2[i]);
    }
  }

  void Sort(vector<int> &v) {
    Merge();
    for (auto [a, b] : compares)
      if (v[a] > v[b]) swap(v[a], v[b]);
  }

  float Fitness() { return fitness; }
  void Printc1() { for (auto [a, b] : c1) cout << a << ' ' << b << '\n'; }
  void Printc2() { for (auto [a, b] : c2) cout << a << ' ' << b << '\n'; }
  void Print() { Merge(); for (auto [a, b] : compares) cout << a << ' ' << b << '\n'; }
};

class GeneticAlgorithm {
private:
  vector<SortingNetwork> population;
  SortingNetwork bestNetwork;
  vector<pair<double,double>> roulette;
  int population_size;
  int crossover_rate;
  int mutation_rate;
  int best_individual;
  float best_score;
  int test_input_size;
  int compare_size;
public:
  GeneticAlgorithm(int popsize, int crossover, int mutation, int inputsize, int comparesize, int testinputsize) {
    population_size = popsize;
    crossover_rate = crossover;
    mutation_rate = mutation;
    compare_size = comparesize;
    // making population
    for (int i = 0; i < popsize; i++)
      population.push_back(SortingNetwork(comparesize, inputsize));
  }

  void Evaluate() {
    // evaluate fitness of each network
    for (auto &network : population)
      network.EvaluateFitness();
    // sort population by fitness value
    sort(population.rbegin(), population.rend(), [](auto &left, auto &right) {
      return left.Fitness() < right.Fitness();
    });
  }

  int Selection() {  // roulette selection
    double r = random(0.0, 100.0);
    int ok = 0, ng = population_size;
    while (abs(ok-ng) > 1) {
      int mid = (ok+ng) / 2;
      auto a = roulette[mid].first;
      if (r < a) ng = mid;
      else ok = mid;
    }
    return ok;
  }

  void Crossover() {
    // sum of fitness
    double sum = 0;
    for (auto individual : population) sum += individual.Fitness();
    // make roulette
    roulette.assign(population_size, {0, 0});
    roulette[0].first = 0;
    roulette.back().second = 100.0;
    for (int i = 0; i+1 < population_size; i++) {
      double p = population[i].Fitness() / sum * 100;
      p += roulette[i].first;
      roulette[i].second = p;
      roulette[i+1].first = p;
    }
    // new sedai
    vector<SortingNetwork> new_population(population_size);
    int i = 0;
    while (i < crossover_rate) {
      // select two parents
      int a = Selection(), b = Selection();
      int cut = random(1, compare_size-1);
      population[a].Cross(population[b], cut);
      new_population[i++] = population[a];
      new_population[i++] = population[b];
    }
    for (int j = 0; i < population_size; i++, j++)
      new_population[i] = population[j];
    swap(population, new_population);
  }

  void Mutation() {
  }

  SortingNetwork GetBestNetwork() {
    Evaluate();
    return population[0];
  }

  void Print() {
    for (auto x : population) {
      cout << x.Fitness() << '\n';
      x.Print();
      cout << '\n';
    }
  }
};

void Test(SortingNetwork &sn, int n) {
  vector<int> a(n);
  iota(a.begin(), a.end(), 0);
  for (int i = 0; i < n; i++) {
    int r = random(0, n);
    swap(a[i], a[r]);
  }
  auto b = a;
  sort(b.begin(), b.end());
  sn.Sort(a);
  for (int i = 0; i < n; i++) {
    cout << b[i] << ' ' << a[i] << '\n';
  }
}

signed main() {
  const int popsize = 10;
  const int crossover = popsize/2;
  const int mutation = 10;
  const int inputsize = 4;
  const int comparesize = inputsize;
  const int testinputsize = inputsize*50;
  const int max_iterations = 0;

  GeneticAlgorithm ga(popsize, crossover, mutation, inputsize, comparesize, testinputsize);
  ga.Evaluate();
  for (int it = 0; it < max_iterations; it++) {
    ga.Crossover();
    ga.Mutation();
    ga.Evaluate();
  }
  // ga.Print();
  auto sn = ga.GetBestNetwork();
  sn.Print();
  cout << '\n';
  Test(sn, inputsize);
}
