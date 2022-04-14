#ifndef __CLRS4_BINARY_SEARCH_H__
#define __CLRS4_BINARY_SEARCH_H__

#include <vector>

namespace frozenca {

using namespace std;

bool binary_search_func(const vector<int>& A, int T) {
    long l = 0;
    long r = ssize(A);
    while (l < r) {
        auto m = l + (r - l) / 2;
        if (A[m] < T) {
            l = m + 1;
        } else if (A[m] > T) {
            r = m;
        } else {
            return true;
        }
    }
    return false;
}

long binary_search_lower_bound(const vector<int>& A, int T, long l, long r) {
    while (l < r) {
        auto m = l + (r - l) / 2;
        if (A[m] < T) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    return l;
}

} // namespace frozenca

#endif //__CLRS4_BINARY_SEARCH_H__
