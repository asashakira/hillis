#include <bits/stdc++.h>
using namespace std;

template<typename T>
T random(T from, T to) {
  random_device rand_dev;
  mt19937 generator(rand_dev());
  uniform_real_distribution<double> distribution(from, to);
  return distribution(generator);
}

signed main() {
}
