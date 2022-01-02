#ifndef SORTINGNETWORK_H
#define SOTRINGNETWORK_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;

class SortingNetwork {
private:
  vector<pair<int,int>> c1, c2, compares;
  float fitness = 0;
  int input_size;
  int compare_size;
  void Merge();
public:
  SortingNetwork();
  SortingNetwork(int size, int inputsize);
  SortingNetwork(vector<pair<int,int>> const &c);
  void EvaluateFitness(vector<pair<vector<int>,int>> &tests);
  void Crossover(SortingNetwork &b);
  void Mutate(int mutation_rate);
  void Sort(vector<int> &v);
  int Size() { return compares.size(); }
  float Fitness() { return fitness; }
  void Printc1() { for (auto [a, b] : c1) cout << a << ' ' << b << '\n'; }
  void Printc2() { for (auto [a, b] : c2) cout << a << ' ' << b << '\n'; }
  void Print() { Merge(); for (auto [a, b] : compares) cout << a << ' ' << b << '\n'; }
};

#endif
