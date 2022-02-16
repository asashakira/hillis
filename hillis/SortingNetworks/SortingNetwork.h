#pragma once

#include <iostream>
#include <vector>

using namespace std;

class SortingNetwork {
private:
  vector<pair<int,int>> compares, c1, c2, g1, g2;
  float fitness=0;
  int input_size;
  int compare_size;
  int mutation_rate;
  double accuracy=-1.0;
public:
  SortingNetwork();
  SortingNetwork(int size, int inputsize, int mutation);
  bool operator<(SortingNetwork &sn);
  void Merge();
  void CreateGametes();
  void Crossover(SortingNetwork &b);
  void Mutate();
  void Sort(vector<int> &v);
  int Size() { return compares.size(); }
  double Accuracy() { return accuracy; }
  float Fitness() { return fitness; }
  void SetFitness(float f) { fitness = f; }
  void SetAccuracy(double a) { accuracy = a; }
  double Test();
  void Printc1();
  void Printc2();
  void Print();
};
