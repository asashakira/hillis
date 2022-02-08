#include "Random.h"
#include "Coevolution.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <future>

using namespace std;

void Test(SortingNetwork &sn, int n) {
  vector<int> a(n);
  iota(a.begin(), a.end(), 1);
  for (int i = 0; i < n; i++) {
    int r = fastrng() % n;
    swap(a[i], a[r]);
  }
  auto b = a;
  sort(b.begin(), b.end());
  sn.Sort(a);
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (a[i] == b[i]) count++;
    cout << b[i] << ' ' << a[i] << ' ' << (a[i] == b[i] ? "OK" : "NG") << '\n';
  }
  cout << (float)count / n * 100 << "%\n";
  cout << "Size: " << sn.Size() << '\n';
}

signed main() {
  // const int popsize = 65536; // must be rootable
  const int popsize = 50*50; // must be rootable
  const int crossover = popsize / 2;
  const int mutation = 1000;
  const int inputsize = 16;
  const int comparesize = 60;
  const int testsize = 256;
  const int max_generation = 500;
}
