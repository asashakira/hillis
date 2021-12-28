#include <bits/stdc++.h>
using namespace std;

void milkshake() {
  map<int,int> mp;
  for (int i = 0; i < 5; i++) {
    mp[i] = i*i;
  }
  for (auto it = mp.rbegin(); it != mp.rend(); it++)
    cout << it->first << '\n';
}

signed main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout << fixed << setprecision(20);
  int tt=1;
  while(tt--) milkshake();
  return 0;
}
