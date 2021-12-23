#include <bits/stdc++.h>
using namespace std;

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

signed main() {
  int ni = 2, nh = 2, no = 1;
  int N = ni+nh+no;
  vector C(N, vector<int>(N+1));
  for (int j = ni; j < N; j++) {
    for (int k = 0; k < ni+nh; k++) {
      if (j <= k) continue;
      C[j][k] = 1;
    }
    if (j >= ni) C[j][N] = 1;
  }
  print2d(C);
}
