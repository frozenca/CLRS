#ifndef __CLRS4_HIRE_ASSISTANT_H__
#define __CLRS4_HIRE_ASSISTANT_H__

#include <common.h>
#include <limits>
#include <vector>

namespace frozenca {

using namespace std;

vector<index_t> hire_assistant(const vector<int> &candidates) {
  auto n = ssize(candidates);
  int best = numeric_limits<int>::min();
  vector<index_t> hired;
  for (index_t i = 0; i < n; ++i) {
    if (candidates[i] > best) {
      best = candidates[i];
      hired.push_back(best);
    }
  }
  return hired;
}

} // namespace frozenca

#endif //__CLRS4_HIRE_ASSISTANT_H__
