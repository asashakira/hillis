#pragma once

#include "SortingNetwork.h"
#include "TestCases.h"

using namespace std;

class Coevolution {
private:
  vector<vector<SortingNetwork>> host;
  vector<vector<TestCases>> parasite;
  int population_size;
  int height, width;
  int crossover_rate;
  int mutation_rate;
  int input_size;
  int compare_size;
  int test_size;
public:
  Coevolution(int popsize, int crossover, int mutation, int inputsize, int comparesize, int testcases);
  void CalculateFitness(SortingNetwork &sn, TestCases &tc);
  void Evaluate();
  void Selection();
  SortingNetwork GetBestNetwork();
  float AverageFitness();
  void Print();
  void PrintPopulation();
};
