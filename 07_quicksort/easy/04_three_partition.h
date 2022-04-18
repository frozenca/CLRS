#ifndef __CLRS4_THREE_PARTITION_H__
#define __CLRS4_THREE_PARTITION_H__

#include <05_probabilistic_analysis_and_randomized_algorithms/easy/01_random_gen.h>
#include <algorithm>
#include <cassert>
#include <common.h>


namespace frozenca {

using namespace std;

index_t partition_three(vector<int> &A, index_t p, index_t r) {
  vector<index_t> temp;
  temp.push_back(random_gen(p, r - 1));
  temp.push_back(random_gen(p, r - 1));
  temp.push_back(random_gen(p, r - 1));
  ranges::sort(temp);
  index_t idx = temp[1];

  auto x = A[idx];
  auto i = p;
  for (auto j = p; j < r; j++) {
    if (A[j] >= x) {
      swap(A[i], A[j]);
      i++;
    }
  }
  swap(A[i], A[r]);
  return i;
}

} // namespace frozenca

#endif //__CLRS4_THREE_PARTITION_H__
