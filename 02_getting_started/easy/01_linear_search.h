#ifndef __CLRS4_LINEAR_SEARCH_EASY_H__
#define __CLRS4_LINEAR_SEARCH_EASY_H__

#include <vector>

namespace frozenca {

using namespace std;

long linear_search(const vector<int> &A, int x) {
  for (long i = 0; i < ssize(A); ++i) {
    if (A[i] == x) {
      return i;
    }
  }
  return -1;
}

} // namespace frozenca

#endif //__CLRS4_LINEAR_SEARCH_EASY_H__
