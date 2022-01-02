#include "SortingNetwork.h"
#include "Random.h"
using namespace std;

void SortingNetwork::Merge() {
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
SortingNetwork::SortingNetwork() {}
SortingNetwork::SortingNetwork(int size, int inputsize) : c1(size), c2(size) {
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
SortingNetwork::SortingNetwork(vector<pair<int,int>> const &c) { compares = c; }

void SortingNetwork::EvaluateFitness(vector<pair<vector<int>,int>> &tests) {
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

void SortingNetwork::Crossover(SortingNetwork &b) {
  for (int i = 0; i < compare_size; i+=4) {
    int crossover_point = fastrng() % 4;
    for (int j = 0; j < crossover_point; j++) {
      c1[i+j] = b.c1[i+j];
      c2[i+j] = b.c2[i+j];
    }
  }
  // int crossover_point = fastrng() % input_size;
  // for (int i = 0; i < crossover_point; i++) {
    // c1[i] = b.c1[i];
    // c2[i] = b.c2[i];
  // }
  c2 = b.c2;
}

void SortingNetwork::Mutate(int mutation_rate) {
  for (auto &[a, b] : compares) {
    // int flip = 1;
    // for (int i = 0; i < 8; i++) {
      // int r = fastrng() % mutation_rate;
      // if (!r) (i < 4 ? a : b) ^= flip;
      // flip <<= 1;
      // if (flip > 8) flip = 1;
    // }
    if (fastrng() % mutation_rate) a = fastrng() % input_size;
    if (fastrng() % mutation_rate) b = fastrng() % input_size;
  }
}

void SortingNetwork::Sort(vector<int> &v) {
  Merge();
  for (auto [a, b] : compares) {
    if (a > b) swap(a, b);
    if (v[a] > v[b]) swap(v[a], v[b]);
  }
}
