#include "SortingNetwork.h"
#include "Random.h"

#include <algorithm>
#include <numeric>

using namespace std;

void SortingNetwork::Merge() {
  compares.clear();
  for (int i = 0; i < 2*compare_size; i+=2) {
    if (c1[i]== c2[i] and c1[i+1] == c2[i+1]) {
      if (c1[i] == c1[i+1]) continue;
      compares.push_back(c1[i]);
      compares.push_back(c1[i+1]);
    } else {
      if (c1[i] != c1[i+1]) {
        compares.push_back(c1[i]);
        compares.push_back(c1[i+1]);
      }
      if (c2[i] != c2[i+1]) {
        compares.push_back(c2[i]);
        compares.push_back(c2[i+1]);
      }
    }
  }
}

SortingNetwork::SortingNetwork() {}
SortingNetwork::SortingNetwork(int size, int inputsize) : c1(size*2), c2(size*2), g1(size*2), g2(size*2) {
  input_size = inputsize;
  compare_size = size;
  if (inputsize == 16) {
    c1 = {
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
      0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15,
      0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15,
      0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15
    };
    c1.resize(size*2);

    c2 = c1;

    // create random network
    for (int i = 2*8; i < 2*size; i++) {
      c1[i] = fastrng() % inputsize;
      c2[i] = fastrng() % inputsize;
    }
  } else {
    for (int i = 0; i < 2*size; i++) {
      c1[i] = fastrng() % inputsize;
      c2[i] = fastrng() % inputsize;
    }
  }
}

void SortingNetwork::CreateGametes() {
  g1 = c1;
  g2 = c2;
  for (int i = 0; i < 2*compare_size; i+=8) {
    int crossover_point = fastrng() % 8;
    for (int j = i; j < i+crossover_point and j < 2*compare_size; j++)
      swap(g1[i], g2[i]);
  }
}

void SortingNetwork::Crossover(SortingNetwork &b) {
  c1 = g1;
  c2 = b.g2;
  b.c1 = b.g1;
  b.c2 = g2;
}

void SortingNetwork::Mutate(int mutation_rate) {
  for (int i = 0; i < Size(); i+=2) {
    int &a = compares[i];
    int &b = compares[i+1];
    int flip = 1;
    for (int i = 0; i < 8; i++) {
      int r = fastrng() % mutation_rate;
      if (!r) (i < 4 ? a : b) ^= flip;
      flip <<= 1;
      if (flip > 8) flip = 1;
    }
    // if (fastrng() % mutation_rate) a = fastrng() % input_size;
    // if (fastrng() % mutation_rate) b = fastrng() % input_size;
  }
}

void SortingNetwork::Sort(vector<int> &v) {
  Merge();
  for (int i = 0; i < Size(); i++) {
    int a = compares[i];
    int b = compares[i+1];
    if (a > b) swap(a, b);
    if (v[a] > v[b]) swap(v[a], v[b]);
  }
}

void SortingNetwork::Printc1() { for (int i = 0; i < compare_size*2; i+=2) cout << c1[i] << ',' << c1[i+1] << '\n'; cout << '\n'; }
void SortingNetwork::Printc2() { for (int i = 0; i < compare_size*2; i+=2) cout << c2[i] << ',' << c2[i+1] << '\n'; cout << '\n'; }
void SortingNetwork::Print() { Merge(); for (int i = 0; i < Size(); i+=2) cout << compares[i] << ',' << compares[i+1] << '\n'; cout << '\n'; }
