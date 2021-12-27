#include <bits/stdc++.h>
using namespace std;

void milkshake() {
  pair<int,int> p{1, 2};
  if (true) {
    auto [a, b] = p;
    swap(a, b);
  }
  cout << p.first << ' ' << p.second << '\n';
}

signed main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout << fixed << setprecision(20);
  int tt=1;
  while(tt--) milkshake();
  return 0;
}
