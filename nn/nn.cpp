#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <math.h>
#include <random>

using namespace std;

#define POP_SIZE 50
#define EPOCHS 5000

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

class NN {
  private:
  vector<vector<int>> X;
  vector<int> Y;
  vector<vector<int>> C;
  const int N;
  const double lr, m;
  int epochs;

  public:
  NN(vector<vector<int>> c, vector<vector<int>> x, vector<int> y, double lr=0.4, double m=0.8, int epochs=EPOCHS) : 
    N(c.size()), lr(lr), m(m), epochs(epochs) {
    C = c;
    X = x;
    Y = y;
  }

  void forward(vector<vector<double>> &v, vector<vector<double>> w) {
    for (int i = 0; i < (int)X.size(); i++) {
      for (int j = 2; j < N; j++) {
        for (int k = 0; k < N+1; k++) {
          v[i][j] += v[i][k]*w[j][k];
        }
        v[i][j] = sigmoid(v[i][j]);
      }
    }
  }

  void backward(vector<vector<double>> &v, vector<vector<double>> &w, vector<vector<double>> &pdw) {
    vector dw(N, vector<double>(N+1));
    for (int i = 0; i < (int)X.size(); i++) {
      double r = (Y[i]-v[i][N-1]) * d_sigmoid(v[i][N-1]);
      for (int j = 0; j < N+1; j++) {
        if (C.back()[j] == 1)
          dw.back()[j] += r*v[i][j];
      }
      for (int j = 2; j < (int)X.size(); j++) {
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

  double train() {
    vector w(N, vector<double>(N+1));
    auto pdw = w;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N+1; j++) {
        if (C[i][j]) w[i][j] = random(0.0, 1.0);
      }
    }
    double error = 0;
    while (epochs--) {
      vector v(4, vector<double>(N+1));
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
          v[i][j] = X[i][j];
        }
        v[i][N] = 1;
      }

      forward(v, w);
      backward(v, w, pdw);

      if (epochs == 0) {
        test(v);
        for (int i = 0; i < 4; i++) {
          error += pow(Y[i]-v[i][N-1], 2);
        }
      }
    }
    return 1.0-error;
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

  vector<vector<int>> c {
    {  0,   0,   0,   0,   0,   0},
    {  0,   0,   0,   0,   0,   0},
    {  1,   1,   0,   0,   0,   1},
    {  1,   1,   0,   0,   0,   0},
    {  0,   0,   1,   1,   0,   1},
  };
  NN nn(c, x, y);
  cout << nn.train() << '\n';
}
