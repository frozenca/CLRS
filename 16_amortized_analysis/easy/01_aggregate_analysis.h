#ifndef __CLRS4_AGGREGATE_ANALYSIS_H__
#define __CLRS4_AGGREGATE_ANALYSIS_H__

#include <cassert>
#include <common.h>
#include <stack>
#include <vector>

namespace frozenca {

using namespace std;

void multipop(stack<int> &s, index_t k) {
  while (!s.empty() && k >= 0) {
    s.pop();
    --k;
  }
}

void multipush(stack<int> &s, index_t k) {
  while (k >= 0) {
    s.push(1);
    --k;
  }
}

void increment(vector<int> &A) {
  index_t i = 0;
  while (i < ssize(A) && A[i]) {
    A[i] = 0;
    ++i;
  }
  if (i < ssize(A)) {
    A[i] = 1;
  }
}

void decrement(vector<int> &A) {
  index_t i = 0;
  while (i < ssize(A) && !A[i]) {
    A[i] = 1;
    ++i;
  }
  if (i < ssize(A)) {
    A[i] = 0;
  }
}

} // namespace frozenca

#endif //__CLRS4_AGGREGATE_ANALYSIS_H__
