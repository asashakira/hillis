
/*
hillis.cpp
Akira Itai
g++ -std=gnu++17 hillis.cpp -o a
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <random>

using namespace std;

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

#define N 4
#define M 4
#define POP_SIZE 10
#define MAX_GEN 10

class Kotai {
  public:
  vector<vector<int>> c1, c2, c;
  int fitness = 0;

  Kotai() : c1(M, vector<int>(2)), c2(M, vector<int>(2)) {
    for (int i = 0; i < M; i++) {
      c1[i][0] = random(0, N);
      c1[i][1] = random(0, N);
      c2[i][0] = random(0, N);
      c2[i][1] = random(0, N);
    }
    merge();
  }

  void merge() {
    for (int i = 0; i < M; i++) {
      if (c1[i][0] == c2[i][0] and c1[i][1] == c2[i][1]) {
        c.push_back(c1[i]);
      } else {
        c.push_back(c1[i]);
        c.push_back(c2[i]);
      }
    }
  }

  Kotai cross(Kotai b, int cut) {
    Kotai nk; // new kotai
    for (int i = 0; i < cut; i++) {
      nk.c1[i] = c1[i];
      nk.c2[i] = c2[i];
    }
    for (int i = cut; i < M; i++) {
      nk.c1[i] = b.c1[i];
      nk.c2[i] = b.c2[i];
    }
    merge();
    return nk;
  }

  void calc_fitness() {
    // making test case
    map<vector<int>,int> mp;
    int tests = 0;
    while (tests < 1) {
      vector<int> a(N);
      int zeros = 0;
      for (int i = 0; i < N; i++) {
        a[i] = random(0, 2);
        if (a[i] == 0) zeros++;
      }
      if (mp.find(a) != mp.end()) continue;
      tests++;

      // apply sorting network
      for (int i = 0; i < (int)c.size(); i++) {
        if (a[c[i][0]] > a[c[i][1]])
          swap(a[c[i][0]], a[c[i][1]]);
      }

      // calc fit
      for (int i = 0; i < N; i++) {
        if (zeros) {
          fitness += (a[i] == 0 ? 1 : 0);
          zeros--;
        } else {
          fitness += (a[i] == 1 ? 1 : 0);
        }
      }
    }
  }

  int size() { return M; }
  void print() {
    for (int i = 0; i < (int)c.size(); i++) {
      for (int j = 0; j < (int)c[i].size(); j++) {
        cout << c[i][j] << ' ';
      }
      cout << '\n';
    }
  }
};

int selection(vector<pair<double,double>> roulette) {
  double r = random(0.0, 100.0);
  int ok = 0, ng = POP_SIZE;
  while (abs(ok-ng) > 1) {
    int mid = (ok+ng) / 2;
    auto a = roulette[mid].first;
    if (r < a) ng = mid;
    else ok = mid;
  }
  return ok;
}

vector<Kotai> crossover(vector<Kotai> s) {
  // sum of fitness
  double sum = 0;
  for (auto k : s) sum += k.fitness;
  // slice pie
  vector<pair<double,double>> roulette(POP_SIZE);
  roulette[0].first = 0;
  roulette.back().second = 100.0;
  for (int i = 0; i+1 < POP_SIZE; i++) {
    double p = s[i].fitness / sum * 100;
    p += roulette[i].first;
    roulette[i].second = p;
    roulette[i+1].first = p;
  }

  vector<Kotai> ns(POP_SIZE);
  int i = 0;
  while (i < POP_SIZE/2) {
    int a = selection(roulette);
    int b = selection(roulette);
    int cut = random(1, s[a].size());
    a = 0; b = 1;
    cut = 1;
    ns[i++] = s[a].cross(s[b], cut);
    ns[i++] = s[b].cross(s[a], cut);
  }
  int j = 0;
  while (i < POP_SIZE) {
    ns[i] = s[j++];
  }
  return ns;
}

void sortingNetwork(vector<vector<int>> c) {
  // making test case
  vector<int> a(N);
  iota(a.begin(), a.end(), 0);
  // shuffle
  for (int i = 0; i < N; i++) {
    int r = random(0, N);
    swap(a[i], a[r]);
  }

  // apply sorting network
  for (int i = 0; i < (int)c.size(); i++) {
    if (a[c[i][0]] > a[c[i][1]])
      swap(a[c[i][0]], a[c[i][1]]);
  }
  for (auto x : a) cout << x << '\n';
}

signed main() {
  // sedai
  vector<Kotai> sedai(POP_SIZE);

  // MAX_GEN回繰り返す
  for (int g = 0; g < MAX_GEN; g++) {

    // 個体の評価を計算
    for (int i = 0; i < POP_SIZE; i++)
      sedai[i].calc_fitness();

    // 評価が高い順にソート
    sort(sedai.rbegin(), sedai.rend(), [](auto &left, auto &right) {
      return left.fitness < right.fitness;
    });

    // 新世代を交叉・突然変異で作成
    auto new_sedai = crossover(sedai);
    //sedai = mutation(new_sedai);
    sedai = new_sedai;

    cout << '\n';
    for (int i = 0; i < POP_SIZE; i++)
      cout << sedai[i].fitness << '\n';
  }

  return 0;
}
