
/*
hillis1
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
#include <thread>

using namespace std;

// rng [from, to)
template<typename T>
T random(T from, T to) {
  random_device rand_dev;
  mt19937 g(rand_dev());
  uniform_real_distribution<double> d(from, to);
  return d(g);
}

// Marsaglia's xorshf96
// static unsigned long rx=123456789, ry=362436069, rz=521288629;
static unsigned long rx=random(0, 123456789), ry=random(0, 362436069), rz=random(0, 521288629);
unsigned long fastrng(void) {
  unsigned long rt;
  rx ^= rx << 16;
  rx ^= rx >> 5;
  rx ^= rx << 1;
  rt = rx;
  rx = ry;
  ry = rz;
  rz = rt ^ rx ^ ry;
  return rz;
}

class SortingNetwork {
private:
  vector<pair<int,int>> c1, c2, compares;
  float fitness = 0;
  int input_size;
  int compare_size;

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
  SortingNetwork() {}
  SortingNetwork(int size, int inputsize) : c1(size), c2(size) {
    input_size = inputsize;
    compare_size = size;
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
      for (int i = 32; i < size; i++) {
        c1[i].first = fastrng() % inputsize;
        c1[i].second = fastrng() % inputsize;
        c2[i].first = fastrng() % inputsize;
        c2[i].second = fastrng() % inputsize;
        // c1[i].first = random(0, inputsize);
        // c1[i].second = random(0, inputsize);
        // c2[i].first = random(0, inputsize);
        // c2[i].second = random(0, inputsize);
      }
    } else {
      for (int i = 0; i < size; i++) {
        c1[i].first = fastrng() % inputsize;
        c1[i].second = fastrng() % inputsize;
        c2[i].first = fastrng() % inputsize;
        c2[i].second = fastrng() % inputsize;
      }
    }
  }
  SortingNetwork(vector<pair<int,int>> const &c) { compares = c; }

  void EvaluateFitness(vector<pair<vector<int>,int>> &tests) {
    fitness = 0;
    Merge();
    for (auto [v, zeros] : tests) {
      // applying sorting network
      for (auto &[a, b] : compares) {
        if (a > b) swap(a, b);
        if (v[a] > v[b]) swap(v[a], v[b]);
      }
      // calculating fitness
      int ok = 1;
      for (auto x : v) {
        if (zeros) {
          // fitness += x == 0;
          if (x != 0)
            ok = 0;
          zeros--;
        } else {
          // fitness += x == 1;
          if (x != 1)
            ok = 0;
        }
      }
      fitness += ok;
    }
    fitness /= tests.size();
  }

  void Crossover(SortingNetwork &b) {
    int crossover_point = fastrng() % input_size;
    for (int i = 0; i < crossover_point; i++) {
      c1[i] = b.c1[i];
      c2[i] = b.c2[i];
    }
    c2 = b.c2;
  }

  void Mutate(int mutation_rate) {
    for (auto &[a, b] : compares) {
      int flip = 1;
      for (int i = 0; i < 8; i++) {
        int r = fastrng() % mutation_rate;
        if (!r) (i < 4 ? a : b) ^= flip;
        flip <<= 1;
        if (flip > 8) flip = 1;
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
  vector<pair<vector<int>,int>> tests;
  int population_size;
  int height, width;
  int crossover_rate;
  int mutation_rate;
  int input_size;
  int compare_size;
  int test_size;
public:
  GeneticAlgorithm(int popsize, int crossover, int mutation, int inputsize, int comparesize, int testsize) {
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

  void Evaluate() {
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

  void Selection() {
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
  void PrintPopulation() {
    for (auto &p : population) {
      for (auto &x : p)
        cout << x.Fitness() << ' ';
      cout << '\n';
    }
  }
};

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
  for (int i = 0; i < n; i++) {
    cout << b[i] << ' ' << a[i] << ' ' << (a[i] == b[i] ? "OK" : "NG") << '\n';
  }
  cout << "Size: " << sn.Size() << '\n';
}

signed main() {
  // const int popsize = 65536; // must be rootable
  const int popsize = 10000; // must be rootable
  const int crossover = popsize / 2;
  const int mutation = 1000;
  const int inputsize = 16;
  const int comparesize = 60;
  const int testsize = 100;
  const int max_generation = 50;

  GeneticAlgorithm ga(popsize, crossover, mutation, inputsize, comparesize, testsize);
  ga.Evaluate();
  for (int gen = 1; gen <= max_generation; gen++) {
    if (gen % 10 == 0) {
      cout << "gen " << gen << " : ";
      cout << ga.GetBestNetwork().Fitness() << '\n';
    }
    ga.Selection();
    ga.Evaluate();
  }
  // ga.PrintPopulation();
  auto sn = ga.GetBestNetwork();
  // sn.Print(); cout << '\n';
  Test(sn, inputsize);
  cout << sn.Fitness() << '\n';
}
