#ifndef __CLRS4_SUM_ARRAY_EASY_H__
#define __CLRS4_SUM_ARRAY_EASY_H__

#include <common.h>
#include <vector>

namespace frozenca {

using namespace std;

int sum_array(const vector<int> &A) {
  int sum = 0;
  for (index_t i = 0; i < ssize(A); ++i) {
    sum += A[i];
  }
  return sum;
}

} // namespace frozenca

#endif //__CLRS4_SUM_ARRAY_EASY_H__
