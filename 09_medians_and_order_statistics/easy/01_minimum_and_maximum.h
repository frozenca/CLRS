#ifndef __CLRS4_MINIMUM_AND_MAXIMUM_H__
#define __CLRS4_MINIMUM_AND_MAXIMUM_H__

#include <algorithm>
#include <common.h>
#include <limits>
#include <vector>


namespace frozenca {

using namespace std;

int minimum(const vector<int> &A) {
  int min_value = numeric_limits<int>::max();
  for (auto num : A) {
    min_value = min(min_value, num);
  }
  return min_value;
}

int maximum(const vector<int> &A) {
  int max_value = numeric_limits<int>::min();
  for (auto num : A) {
    max_value = max(max_value, num);
  }
  return max_value;
}

} // namespace frozenca

#endif //__CLRS4_MINIMUM_AND_MAXIMUM_H__
