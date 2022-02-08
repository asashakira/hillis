#include "SortingNetwork.h"
#include "Random.h"

#include <algorithm>
#include <numeric>

using namespace std;

SortingNetwork::SortingNetwork() {}
SortingNetwork::SortingNetwork(int size, int inputsize, int mutation) : compares(size*2), c1(size), c2(size), g1(size), g2(size) {
  input_size = inputsize;
  compare_size = size;
  mutation_rate = mutation;
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

void SortingNetwork::Merge() {
  compares.clear();
  for (int i = 0; i < (int)c1.size(); i++) {
    if (c1[i] == c2[i]) {
      if (c1[i].first == c1[i].second) continue;
      compares.push_back(c1[i]);
    } else {
      if (c1[i].first != c1[i].second)
        compares.push_back(c1[i]);
      if (c2[i].first != c2[i].second)
        compares.push_back(c2[i]);
    }
  }
}

void SortingNetwork::CreateGametes() {
  g1 = c1;
  g2 = c2;
  for (int i = 0; i < compare_size; i+=4) {
    int crossover_point = fastrng() % 4;
    for (int j = i; j < i+crossover_point and j < compare_size; j++)
      swap(g1[i], g2[i]);
  }

  SortingNetwork::Mutate();
}

void SortingNetwork::Crossover(SortingNetwork &b) {
  c1 = g1;
  c2 = b.g1;
}

void SortingNetwork::Mutate() {
  for (int i = 0; i < compare_size; i++) {
    if (!(fastrng() % mutation_rate)) g1[i].first = fastrng() % input_size;
    if (!(fastrng() % mutation_rate)) g1[i].second = fastrng() % input_size;
    if (!(fastrng() % mutation_rate)) g2[i].first = fastrng() % input_size;
    if (!(fastrng() % mutation_rate)) g2[i].second = fastrng() % input_size;
  }
}

void SortingNetwork::Sort(vector<int> &t) {
  Merge();
  for (auto [a, b] : compares) {
    if (a > b) swap(a, b);
    if (t[a] > t[b]) swap(t[a], t[b]);
  }
}

void SortingNetwork::Printc1() { for (auto [a, b] : c1) cout << a << ',' << b << ' '; cout << '\n'; }
void SortingNetwork::Printc2() { for (auto [a, b] : c2) cout << a << ',' << b << ' '; cout << '\n'; }
void SortingNetwork::Print() { Merge(); for (auto [a, b] : compares) cout << a << ' ' << b << '\n'; }
