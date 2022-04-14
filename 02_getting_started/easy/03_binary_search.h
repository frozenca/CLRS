#ifndef __CLRS4_BINARY_SEARCH_H__
#define __CLRS4_BINARY_SEARCH_H__

#include <core/common.h>
#include <vector>

namespace frozenca {

using namespace std;

bool binary_search_func(const vector<int>& A, int T) {
    index_t l = 0;
    index_t r = ssize(A);
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

index_t binary_search_lower_bound(const vector<int>& A, int T, index_t l, index_t r) {
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
