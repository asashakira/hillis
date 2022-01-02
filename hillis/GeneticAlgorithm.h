#ifndef GA_H
#define GA_H

#include "SortingNetwork.h"
using namespace std;

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
  GeneticAlgorithm(int popsize, int crossover, int mutation, int inputsize, int comparesize, int testsize);
  void Evaluate();
  void Selection();
  SortingNetwork GetBestNetwork();
  float AverageFitness();
  void Print();
  void PrintPopulation();
};

#endif
