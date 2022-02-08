#pragma once

#include <iostream>
#include <vector>

using namespace std;

class SortingNetwork {
private:
  vector<int> c1, c2, compares, g1, g2;
  float fitness = 0;
  int input_size;
  int compare_size;
public:
  SortingNetwork();
  SortingNetwork(int size, int inputsize);
  void Merge();
  void CreateGametes();
  void Crossover(SortingNetwork &b);
  void Mutate(int mutation_rate);
  void Sort(vector<int> &v);
  void Printc1();
  void Printc2();
  void Print();
  int Size() { return compares.size(); }
  float Fitness() { return fitness; }
  void SetFitness(float f) { fitness = f; }
};