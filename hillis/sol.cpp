
/*
sol.cpp
Akira Itai
g++ -std=gnu++17 sol.cpp -o a
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

    if (inputsize == 16) {
      int j = 0;
      c1[j++] = {0, 1}; c1[j++] = {2, 3}; c1[j++] = {4, 5}; c1[j++] = {6, 7};
      c1[j++] = {8, 9}; c1[j++] = {10, 11}; c1[j++] = {12, 13}; c1[j++] = {14, 15};
      c1[j++] = {0, 2}; c1[j++] = {4, 6}; c1[j++] = {8, 10}; c1[j++] = {12, 14};
      c1[j++] = {1, 3}; c1[j++] = {5, 7}; c1[j++] = {9, 11}; c1[j++] = {13, 15};
      c1[j++] = {0, 4}; c1[j++] = {8, 12}; c1[j++] = {1, 5}; c1[j++] = {9, 13};
      c1[j++] = {2, 6}; c1[j++] = {10, 14}; c1[j++] = {3, 7}; c1[j++] = {11, 15};
      c1[j++] = {0, 8}; c1[j++] = {1, 9}; c1[j++] = {2, 10}; c1[j++] = {3, 11};
      c1[j++] = {4, 12}; c1[j++] = {5, 13}; c1[j++] = {6, 14}; c1[j++] = {7, 15};

      c2 = c1;

      // create random network
      // c1
      for (int i = 32; i < size; i++) {
        auto &[a, b] = c1[i];
        do {
          a = random(0, inputsize);
          b = random(0, inputsize);
        } while (a == b); // re-roll until a != b
      }
      // c2
      for (int i = 32; i < size; i++) {
        auto &[a, b] = c2[i];
        do {
          a = random(0, inputsize);
          b = random(0, inputsize);
        } while (a == b); // re-roll until a != b
      }
    } else {
      // create random network
      // c1
      for (int i = 0; i < size; i++) {
        auto &[a, b] = c1[i];
        do {
          a = random(0, inputsize);
          b = random(0, inputsize);
        } while (a == b); // re-roll until a != b
      }
      // c2
      for (int i = 0; i < size; i++) {
        auto &[a, b] = c2[i];
        do {
          a = random(0, inputsize);
          b = random(0, inputsize);
        } while (a == b); // re-roll until a != b
      }
    }
  }
  SortingNetwork(vector<pair<int,int>> const &c) { compares = c; }

  void EvaluateFitness() {
    fitness = 0;
    map<vector<int>,int> mp; // to not test duplicates
    int max_tests = 100;
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
      int correct = 0;
      for (auto x : v) {
        if (zeros) {
          correct += x == 0 ? 1 : 0;
          zeros--;
        } else {
          correct += x == 1 ? 1 : 0;
        }
      }
      // fitness = max(fitness, (float)correct / input_size);
      fitness += correct;
    }
  }

  SortingNetwork Cross(SortingNetwork const &b, int cut) {
    SortingNetwork new_network(c1.size(), input_size);
    for (int i = 0; i < cut; i++) {
      new_network.c1[i] = c1[i];
      new_network.c2[i] = c2[i];
    }
    for (int i = cut; i < (int)c1.size(); i++) {
      new_network.c1[i] = b.c1[i];
      new_network.c2[i] = b.c2[i];
    }
    return new_network;
  }

  void Mutate(int mutation_rate) {
    for (auto &[a, b] : compares) {
      int flip = 1;
      for (int i = 0; i < 8; i++) {
        double r = random(0.0, 1.0);
        if (r < 1.0/mutation_rate) {
          if (i < 4) {
            a ^= flip;
          } else {
            if (flip > 8) flip >>= i;
            b ^= flip;
          }
        }
        flip <<= 1;
      }
    }
  }

  void Sort(vector<int> &v) {
    Merge();
    for (auto [a, b] : compares) {
      if (a > b) swap(a, b);
      if (v[a] > v[b]) swap(v[a], v[b]);
    }
  }

  int Size() { return compares.size(); }
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
  int roulette_size;
  int crossover_rate;
  int mutation_rate;
  int best_individual;
  float best_score;
  int test_input_size;
  int compare_size;
public:
  GeneticAlgorithm(int popsize, int crossover, int mutation, int inputsize, int comparesize, int testinputsize) {
    population_size = popsize;
    roulette_size = popsize/2;
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
    int ok = 0, ng = population_size / 2;
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
    for (auto networks : population) sum += networks.Fitness();
    // make roulette
    roulette.assign(roulette_size, {0, 0});
    roulette[0].first = 0;
    roulette.back().second = 100.0;
    for (int i = 0; i+1 < roulette_size; i++) {
      double p = population[i].Fitness() / sum * 100;
      p += roulette[i].first;
      roulette[i].second = p;
      roulette[i+1].first = p;
    }
    // new sedai
    auto new_population = population;
    for (int i = population_size - crossover_rate - (population_size-crossover_rate)%2; i < population_size; ) {
      // select two parents
      int a = Selection(), b = Selection();
      // int a = random(0, population_size), b = random(0, population_size);
      int crossover_point = random(1, compare_size-1);
      new_population[i++] = population[a].Cross(population[b], crossover_point);
      new_population[i++] = population[b].Cross(population[a], crossover_point);
    }
    swap(population, new_population);
  }

  void Mutation() {
    for (auto network : population)
      network.Mutate(mutation_rate);
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
  iota(a.begin(), a.end(), 1);
  for (int i = 0; i < n; i++) {
    int r = random(0, n);
    swap(a[i], a[r]);
  }
  auto b = a;
  sort(b.begin(), b.end());
  sn.Sort(a);
  for (int i = 0; i < n; i++) {
    cout << b[i] << ' ' << a[i] << ' ' << (a[i] == b[i] ? "OK" : "NG") << '\n';
  }
  cout << "Size: " << sn.Size() << '\n';
}

signed main() {
  const int popsize = 100;
  const int crossover = popsize/2;
  const int mutation = 1000;
  const int inputsize = 16;
  const int comparesize = 60;
  const int testinputsize = inputsize*50;
  const int max_iterations = 50;

  GeneticAlgorithm ga(popsize, crossover, mutation, inputsize, comparesize, testinputsize);
  ga.Evaluate();
  for (int it = 0; it < max_iterations; it++) {
    ga.Crossover();
    ga.Mutation();
    ga.Evaluate();
  }
  auto sn = ga.GetBestNetwork();
  sn.Print(); cout << '\n';
  Test(sn, inputsize);
}
