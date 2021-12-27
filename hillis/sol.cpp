
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

  int EvaluateFitness() {
    fitness = 0;
    map<vector<int>,int> mp; // to not test duplicates
    int max_tests = 100;
    for (int tests = 0; tests < max_tests; ) {
      vector<int> v(input_size);
      int zeros = 0;
      for (auto &x : v) {
        x = random(0, 2);
        zeros += x == 0;
      }
      // if (mp.find(v) != mp.end()) continue;
      tests++;
      Sort(v); // applying sorting network
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
    return fitness
  }

  SortingNetwork Crossover(SortingNetwork const &b, int cut) {
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
  vector<vector<SortingNetwork>> population;
  vector<vector<bool>> use;
  int population_size;
  int height, width;
  int crossover_rate;
  int mutation_rate;
  int compare_size;
public:
  GeneticAlgorithm(int popsize, int crossover, int mutation, int inputsize, int comparesize) {
    population_size = popsize;
    height = width = sqrt(popsize);
    crossover_rate = crossover;
    mutation_rate = mutation;
    compare_size = comparesize;
    // making population
    population.resize(height);
    for (int i = 0; i < height; i++) {
      population[i].resize(width);
      for (int j = 0; j < width; j++) {
        population[i][j] = SortingNetwork(comparesize, inputsize);
      }
    }
  }

  void Evaluate() {
    map<float,pair<int,int>> mp;
    // evaluate fitness of each network
    for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++) {
        auto fitness = population[i][j].EvaluateFitness();
        mp[fitness] = make_pair(i, j);
      }
    // use top 50% for crossover
    for (auto it = mp.rbegin(); it != mp.rend(); it++) {
      auto [i, j] = mp->second;
      use[i][j] = true;
    }
  }

  int Selection() {
    return 0;
  }

  void Crossover() {
    auto prev_population = population;
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
      }
    }
  }

  void Mutation() {
    for (auto networks : population)
      for (auto network : networks)
        network.Mutate(mutation_rate);
  }

  SortingNetwork GetBestNetwork() {
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

  void Print() {
    for (auto &p : population) {
      for (auto &x : p) {
        cout << x.Fitness() << '\n';
        x.Print();
        cout << '\n';
      }
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
  const int popsize = 100; // must be rootable
  const int crossover = popsize / 2;
  const int mutation = 1000;
  const int inputsize = 16;
  const int comparesize = 60;
  const int max_iterations = 1;

  GeneticAlgorithm ga(popsize, crossover, mutation, inputsize, comparesize);
  ga.Evaluate();
  for (int it = 0; it < max_iterations; it++) {
    ga.Crossover();
    ga.Mutation();
    // ga.Evaluate();
  }
  auto sn = ga.GetBestNetwork();
  sn.Print(); cout << '\n';
  Test(sn, inputsize);
}
