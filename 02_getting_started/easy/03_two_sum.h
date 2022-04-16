#ifndef __CLRS4_TWO_SUM_H__
#define __CLRS4_TWO_SUM_H__

#include <common.h>
#include <algorithm>
#include <vector>

namespace frozenca {

using namespace std;

bool two_sum(vector<int>& A, int key) {
    ranges::sort(A);
    index_t left = 0;
    index_t right = ssize(A) - 1;
    while (left < right) {
        if (A[left] + A[right] < key) {
            ++left;
        } else if (A[left] + A[right] > key) {
            --right;
        } else {
            return true;
        }
    }
    return false;
}

} // namespace frozenca

#endif //__CLRS4_TWO_SUM_H__
