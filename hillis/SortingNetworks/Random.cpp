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

// Marsaglia's xorshf96
// static unsigned long rx=123456789, ry=362436069, rz=521288629;
static unsigned long rx=random(0, 123456789), ry=random(0, 362436069), rz=random(0, 521288629);
unsigned long fastrng(void) {
  unsigned long rt;
  rx ^= rx << 16;
  rx ^= rx >> 5;
  rx ^= rx << 1;
  rt = rx;
  rx = ry;
  ry = rz;
  rz = rt ^ rx ^ ry;
  return rz;
}
