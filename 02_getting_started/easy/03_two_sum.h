#ifndef __CLRS4_TWO_SUM_H__
#define __CLRS4_TWO_SUM_H__

#include <algorithm>
#include <vector>

namespace frozenca {

using namespace std;

bool two_sum(std::vector<int>& A, int key) {
    ranges::sort(A);
    long left = 0;
    long right = ssize(A) - 1;
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
