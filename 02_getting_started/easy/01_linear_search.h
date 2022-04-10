#ifndef __CLRS4_LINEAR_SEARCH_EASY_H__
#define __CLRS4_LINEAR_SEARCH_EASY_H__

#include <vector>

namespace frozenca {

using namespace std;

long linear_search(const vector<int>& A, int x) {
    long n = ssize(A);
    for (long i = 0; i < n; ++i) {
        if (A[i] == x) {
            return i;
        }
    }
    return -1;
}

} // namespace frozenca

#endif //__CLRS4_LINEAR_SEARCH_EASY_H__
