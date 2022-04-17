#ifndef __CLRS4_RANDOM_GEN_H__
#define __CLRS4_RANDOM_GEN_H__

#include <common.h>
#include <cmath>
#include <random>

namespace frozenca {

using namespace std;

index_t random_gen(index_t a, index_t b) {
  if (a > b) {
    return random_gen(b, a);
  }
  if (a == b) {
    return a;
  }
  mt19937 gen(random_device{}());
  bernoulli_distribution dist(0.5);
  float m = a + (b - a) / 2.0f;
  int r = dist(gen);
  if (r) {
    return random_gen(static_cast<index_t>(ceil(m)), b);
  } else {
    return random_gen(a, static_cast<index_t>(floor(m)));
  }
}

} // namespace frozenca

#endif //__CLRS4_RANDOM_GEN_H__
