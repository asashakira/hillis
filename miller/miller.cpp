
/*

miller.cpp
Akira Itai
g++ -std=gnu++17 miller.cpp -o a

*/


#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <random>

using namespace std;

#define INPUT_SIZE 4
#define POP_SIZE 50
#define MAX_GEN 30
#define EPOCHS 1000

const double lr = 0.4;
const double m = 0.8;

// rng [from, to)
template<typename T>
T random(T from, T to) {
  random_device rand_dev;
  mt19937 g(rand_dev());
  uniform_real_distribution<double> d(from, to);
  return d(g);
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

void iota_sort(vector<vector<vector<int>>> &C, vector<double> &errors) {
  vector<int> p(C.size());
  iota(p.begin(), p.end(), 0);
  sort(p.begin(), p.end(), [&](int i, int j) { return errors[i] > errors[j]; });
  vector<double> tmp(C.size());
  transform(p.begin(), p.end(), tmp.begin(), [&](int i){ return errors[i]; });
  swap(errors, tmp);
  auto nC = C;
  transform(p.begin(), p.end(), nC.begin(), [&](int i){ return C[i]; });
  swap(C, nC);
}


double sigmoid(double x) { return 1/(1+exp(-x)); }
double d_sigmoid(double x) { return x*(1-x); }

class Miller {
  private:
  int N, ni, nh, no, cases;
  vector<vector<vector<int>>> C; // connection matrix
  vector<vector<bool>> learnable; // if connection is learnable
  vector<vector<int>> X; // inputs
  vector<int> Y; // outputs

  public:
  Miller(vector<vector<int>> x, vector<int> y, int ni, int nh, int no):
    C(POP_SIZE, vector<vector<int>>(ni+nh+no, vector<int>(ni+nh+no+1))),
    learnable(ni+nh+no, vector<bool>(ni+nh+no+1, false)) {
    N = ni+nh+no;
    ni = ni;
    nh = nh;
    no = no;
    X = x;
    Y = y;
    cases = x.size();

    for (int i = 0; i < POP_SIZE; i++) {
      for (int j = ni; j < N; j++) {
        for (int k = 0; k < ni+nh; k++) {
          if (j <= k) continue;
          C[i][j][k] = random(0, 2);
          learnable[j][k] = true;
        }
        if (j < ni) continue;
        C[i][j][N] = random(0, 2);
        learnable[j][N] = true;
      }
    }
  }

  void forward(vector<vector<double>> &v, vector<vector<double>> w) {
    for (int i = 0; i < cases; i++) {
      for (int j = ni; j < N; j++) {
        for (int k = 0; k < N+1; k++) {
          v[i][j] += v[i][k]*w[j][k];
        }
        v[i][j] = sigmoid(v[i][j]);
      }
    }
  }

  void backward(vector<vector<double>> &v, vector<vector<double>> &w, vector<vector<double>> &pdw, vector<vector<int>> c) {
    vector dw(N, vector<double>(N+1));
    for (int i = 0; i < cases; i++) {
      double r = (Y[i]-v[i][N-1]) * d_sigmoid(v[i][N-1]);
      for (int j = 0; j < N+1; j++) {
        if (c.back()[j] == 1)
          dw.back()[j] += r*v[i][j];
      }
      for (int j = ni; j < ni+nh; j++) {
        for (int k = 0; k < ni; k++) {
          if (c[j][k])
            dw[j][k] += X[i][k] * d_sigmoid(v[i][j]) * w.back()[j] * r;
        }
        if (c[j].back())
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
    for (int i = 0; i < cases; i++) {
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
  double fitness(vector<vector<int>> c) {
    vector w(N, vector<double>(N+1));
    auto pdw = w;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N+1; j++) {
        if (c[i][j]) w[i][j] = random(0.0, 1.0);
      }
    }
    double error = 0;
    int epochs = EPOCHS;
    while (epochs--) {
      vector v(cases, vector<double>(N+1));
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
          v[i][j] = X[i][j];
        }
        v[i][N] = 1;
      }

      forward(v, w);
      backward(v, w, pdw, c);

      if (epochs == 0) {
        for (int i = 0; i < cases; i++) {
          error += pow(Y[i]-v[i][N-1], 2);
        }
      }
    }
    return 1.0-error;
  }

  // roulette selection
  int selection(vector<pair<double,double>> roulette) {
    double r = random(0.0, 100.0);
    int ok = 0, ng = POP_SIZE;
    while (abs(ok-ng) > 1) {
      int mid = (ok+ng) / 2;
      auto [a, b] = roulette[mid];
      if (r < roulette[mid].first) ng = mid;
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
    //for (int i = 0; i < POP_SIZE; i++) { auto [a, b] = roulette[i]; cout << i << ' ' << a << ' ' << b << '\n'; }
    //cout << selection(roulette) << '\n';

    int j = 0;
    while (j < POP_SIZE*0.6) {
      int par1 = selection(roulette);
      int par2 = selection(roulette);
      int swapIdx = random(0, N);
      auto c1 = C[par1];
      auto c2 = C[par2];
      swap(c1[swapIdx], c2[swapIdx]);
      newC[j++] = c1;
    }

    for (int i = 0; j < POP_SIZE; i++, j++) {
      newC[j] = C[i];
    }

    return newC;
  }

  vector<vector<vector<int>>> mutation(vector<vector<vector<int>>> C) {
    for (int i = 0; i < POP_SIZE; i++) {
      for (int j = 0; j < N; j++) {
        for (int k = 0; k < N+1; k++) {
          if (!learnable[j][k]) continue;
          auto r = random(0.0, 1.0);
          if (r > 0.005) continue;
          C[i][j][k] ^= 1;
        }
      }
    }
    return C;
  }

  void run() {
    map<vector<vector<int>>,int> mp;
    for (int it = 0; it < MAX_GEN; it++) {
      // calc fitness for each chromosome
      vector<double> errors;
      for (int i = 0; i < (int)C.size(); i++) {
        errors.push_back(fitness(C[i]));
      }
      for (auto c : C)
        errors.push_back(fitness(c));

      iota_sort(C, errors);

      cout << errors[0] << '\n'; print2d(C[0]);
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
};



signed main() {
  vector<vector<int>> x {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1},
  };
  vector<int> y {0, 1, 1, 0};

  Miller mil(x, y, 2, 2, 1);
  mil.run();
}
