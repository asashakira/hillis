
/*

miller.cpp
Akira Itai
g++ -std=gnu++17 -o a

*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <random>

using namespace std;

#define N 5
#define INPUT_SIZE 4
#define POP_SIZE 50
#define MAX_GEN 10
#define EPOCHS 1000

const double lr = 0.4;
const double m = 0.8;

const int X[][4] {
  {0, 0},
  {0, 1},
  {1, 0},
  {1, 1},
};
const int Y[4] {0, 1, 1, 0};

// rng [from, to)
template<typename T>
T random(T from, T to) {
  random_device rand_dev;
  mt19937 generator(rand_dev());
  uniform_real_distribution<double> distribution(from, to);
  return distribution(generator);
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

double sigmoid(double x) { return 1/(1+exp(-x)); }
double d_sigmoid(double x) { return x*(1-x); }

void forward(vector<vector<double>> &v, vector<vector<double>> w) {
  for (int i = 0; i < 4; i++) {
    for (int j = 2; j < N; j++) {
      for (int k = 0; k < N+1; k++) {
        v[i][j] += v[i][k]*w[j][k];
      }
      v[i][j] = sigmoid(v[i][j]);
    }
  }
}

void backward(vector<vector<double>> &v, vector<vector<double>> &w, vector<vector<double>> &pdw, vector<vector<int>> C) {
  vector dw(N, vector<double>(N+1));
  for (int i = 0; i < 4; i++) {
    double r = (Y[i]-v[i][N-1]) * d_sigmoid(v[i][N-1]);
    for (int j = 0; j < N+1; j++) {
      if (C.back()[j] == 1)
        dw.back()[j] += r*v[i][j];
    }
    for (int j = 2; j < 4; j++) {
      for (int k = 0; k < 2; k++) {
        if (C[j][k])
          dw[j][k] += X[i][k] * d_sigmoid(v[i][j]) * w.back()[j] * r;
      }
      if (C[j].back())
        dw[j].back() += d_sigmoid(v[i][j]) * w.back()[j] * r;
    }
  }

  // update weights
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N+1; j++) {
      w[i][j] += dw[i][j]*lr + pdw[i][j]*m;
    }
  }
  pdw = dw;
}

void test(vector<vector<double>> v) {
  for (int i = 0; i < 4; i++) {
    auto x = X[i];
    auto y = Y[i];
    double z = v[i][N-1];
    printf("[%d %d] -> [%d] -> [%.5lf] ", x[0], x[1], y, z);
    if (y == 0)
      cout << (z < 0.5 ? "OK" : "NG") << '\n';
    else
      cout << (z < 0.5 ? "NG" : "OK") << '\n';
  }
}

// calc fitness
double fitness(vector<vector<int>> C) {
  vector w(N, vector<double>(N+1));
  auto pdw = w;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N+1; j++) {
      if (C[i][j]) w[i][j] = random(0.0, 1.0);
    }
  }
  double error = 0;
  int epochs = EPOCHS;
  while (epochs--) {
    vector v(4, vector<double>(N+1));
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 2; j++) {
        v[i][j] = X[i][j];
      }
      v[i][N] = 1;
    }

    forward(v, w);
    backward(v, w, pdw, C);

    if (epochs == 0) {
      for (int i = 0; i < 4; i++) {
        error += pow(Y[i]-v[i][N-1], 2);
      }
    }
  }
  return error;
}

// roulette selection
int selection(vector<pair<double,double>> roulette) {
  double r = random(0.0, 100.0);
  int ok = 0, ng = POP_SIZE;
  while (abs(ok-ng) > 1) {
    int mid = (ok+ng) / 2;
    auto [a, b] = roulette[mid];
    if (r < a) ng = mid;
    else ok = mid;
  }
  return ok;
}

// crossover
vector<vector<vector<int>>> crossover(vector<vector<vector<int>>> C, vector<double> fit) {
  // new population
  vector<vector<vector<int>>> newC(POP_SIZE, vector<vector<int>>(N, vector<int>(N+1)));

  // sum of fitness
  double sum = 0;
  for (auto f : fit) sum += f;

  // slice pie
  vector<pair<double,double>> roulette(POP_SIZE);
  roulette[0].first = 0;
  roulette.back().second = 100.0;
  for (int i = 0; i+1 < POP_SIZE; i++) {
    double p = fit[i] / sum * 100;
    p += roulette[i].first;
    roulette[i].second = p;
    roulette[i+1].first = p;
  }
//  for (int i = 0; i < POP_SIZE; i++) { auto [a, b] = roulette[i]; cout << i << ' ' << a << ' ' << b << '\n'; }
//  cout << selection(roulette) << '\n';

  int j = 0;
  while (j < POP_SIZE*6/10) {
    int par1 = selection(roulette);
    int par2 = selection(roulette);
    int swapIdx = random(0, N);
    auto c1 = C[par1];
    auto c2 = C[par2];
    swap(c1[swapIdx], c2[swapIdx]);
    newC[j++] = c1;
  }

  for (int i = 0; j < POP_SIZE; i++) {
    newC[j++] = C[i];
  }

  return newC;
}

vector<vector<vector<int>>> mutation(vector<vector<vector<int>>> C) {
  for (int i = 0; i < POP_SIZE; i++) {
    for (int j = 2; j < N; j++) {
      for (int k = 0; k < N+1; k++) {
        if (j == 2 or j == 3) if (k == 2 or k == 3 or k == 4) continue;
        if (j == 4 and k == 4) continue;

        auto r = random(0.0, 1.0);
        if (r > 0.005) continue;
        C[i][j][k] ^= 1;
      }
    }
  }
  return C;
}

signed main() {
  // initial population
  int ni = 2, nh = 2, no = 1;
  vector C(POP_SIZE, vector<vector<int>>(N, vector<int>(N+1)));
  for (int i = 0; i < POP_SIZE; i++) {
    for (int j = ni; j < N; j++) {
      for (int k = 0; k < N; k++) {
        if (j <= k) continue;
        C[i][j][k] = random(0, 2);
      }
      if (j >= ni) C[i][j][N] = random(0, 2);
    }
  }

  map<vector<vector<int>>,int> mp;
  int gen = 0;
  while (gen++ < MAX_GEN) {
    // calc fitness for each chromosome
    vector<double> errors;
    for (int i = 0; i < (int)C.size(); i++) {
      errors.push_back(fitness(C[i]));
    }

    // sort
    vector<int> p(C.size());
    iota(p.begin(), p.end(), 0);
    sort(p.begin(), p.end(), [&](int i, int j) { return errors[i] < errors[j]; });
    vector<double> tmp(C.size());
    transform(p.begin(), p.end(), tmp.begin(), [&](int i){ return errors[i]; });
    swap(errors, tmp);
    auto nC = C;
    transform(p.begin(), p.end(), nC.begin(), [&](int i){ return C[i]; });
    swap(C, nC);

    double chancePerformance = 1.0;
    for (auto &e : errors) e = chancePerformance - e;

    //cout << errors[0] << '\n'; print2d(C[0]);
    for (int i = 0; i < POP_SIZE; i++) mp[C[i]]++;

    C = crossover(C, errors);
    C = mutation(C);
  }

  vector<pair<int,vector<vector<int>>>> ans;
  for (auto it = mp.begin(); it != mp.end(); it++) {
    auto c = it->first;
    ans.push_back({it->second, it->first});
  }
  sort(ans.begin(), ans.end());
  auto c = ans.back().second;
    for (int i = 0; i < (int)c.size(); i++) {
      for (int j = 0; j < (int)c[i].size(); j++) {
        cout << c[i][j] << ' ';
      }
      cout << '\n';
    }
    cout << ans.back().first << '\n';
}
