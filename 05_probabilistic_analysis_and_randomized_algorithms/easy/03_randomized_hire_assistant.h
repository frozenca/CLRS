#ifndef __CLRS4_RANDOMIZED_HIRE_ASSISTANT_H__
#define __CLRS4_RANDOMIZED_HIRE_ASSISTANT_H__

#include "01_hire_assistant.h"
#include "01_random_gen.h"
#include <algorithm>
#include <common.h>

namespace frozenca {

using namespace std;

void randomly_permute(vector<int> &A) { // not any better than std::shuffle...
  auto n = ssize(A);
  for (index_t i = 0; i < n; ++i) {
    swap(A[i], A[random_gen(i, n - 1)]);
  }
}

void randomly_permute_rewrite(vector<int> &A) {
  auto n = ssize(A);
  if (n == 0) {
    return;
  }
  swap(A[0], A[random_gen(0, n - 1)]);
  for (index_t i = 1; i < n; ++i) {
    swap(A[i], A[random_gen(i, n - 1)]);
  }
}

void randomly_permute_without_identity(vector<int> &A) {
  auto n = ssize(A);
  for (index_t i = 0; i < n - 1; ++i) {
    swap(A[i], A[random_gen(i + 1, n - 1)]);
  }
}

void randomly_permute_with_all(vector<int> &A) {
  auto n = ssize(A);
  for (index_t i = 0; i < n; ++i) {
    swap(A[i], A[random_gen(0, n - 1)]);
  }
}

void randomly_permute_by_cycle(vector<int> &A) {
  auto n = ssize(A);
  if (n == 0) {
    return;
  }
  vector<int> B(n);
  auto offset = random_gen(0, n - 1);
  for (index_t i = 0; i < n; ++i) {
    auto dest = i + offset;
    if (dest >= n) {
      dest -= n;
    }
    B[dest] = A[i];
  }
  swap(A, B);
}

vector<index_t> randomized_hire_assistant(vector<int> &candidates) {
  randomly_permute(candidates);
  return hire_assistant(candidates);
}

} // namespace frozenca

#endif //__CLRS4_RANDOMIZED_HIRE_ASSISTANT_H__
