#ifndef __CLRS4_DYNAMIC_MULTISET_H__
#define __CLRS4_DYNAMIC_MULTISET_H__

#include <algorithm>
#include <cassert>
#include <common.h>
#include <ranges>
#include <vector>

namespace frozenca {

using namespace std;

void delete_larger_half(vector<int> &v) {
  const auto n = ssize(v);
  ranges::nth_element(v, v.begin() + n / 2);
  v.resize(n / 2);
}

} // namespace frozenca

#endif //__CLRS4_DYNAMIC_MULTISET_H__
