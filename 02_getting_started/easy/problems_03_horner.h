#ifndef __CLRS4_PROBLEMS_02_03_H__
#define __CLRS4_PROBLEMS_02_03_H__

#include <vector>

namespace frozenca {

using namespace std;

float horner(const std::vector<float> &A, float x) {
  float value = 0;
  for (long i = 0; i < ssize(A); ++i) {
    value = A[i] + x * value;
  }
  return value;
}

} // namespace frozenca

#endif //__CLRS4_PROBLEMS_02_03_H__
