#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

void milkshake() {
}

signed main() {
  auto start = high_resolution_clock::now();
  for (int i = 0; i < 256*256; i++)
    for (int j = 0; j < 5000; j++)
      continue;
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  cout << duration.count() << '\n';

  return 0;
}
