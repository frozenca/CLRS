#ifndef __CLRS4_ACCOUNTING_METHOD_H__
#define __CLRS4_ACCOUNTING_METHOD_H__

#include <cassert>
#include <common.h>
#include <stack>
#include <vector>

namespace frozenca {

using namespace std;

stack<int> clone(const stack<int> &s) { return s; }

void increment_max(pair<vector<int>, index_t> &A) {
  index_t i = 0;
  while (i < ssize(A.first) && A.first[i]) {
    A.first[i] = 0;
    ++i;
  }
  if (i < ssize(A.first)) {
    A.first[i] = 1;
    if (i > A.second) {
      A.second = i;
    }
  } else {
    A.second = -1;
  }
}

void reset_counter(pair<vector<int>, index_t> &A) {
  for (index_t i = 0; i < A.second; ++i) {
    A.first[i] = 0;
  }
  A.second = -1;
}

} // namespace frozenca

#endif //__CLRS4_ACCOUNTING_METHOD_H__
