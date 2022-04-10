#ifndef __CLRS4_INSERTION_SORT_EASY_H__
#define __CLRS4_INSERTION_SORT_EASY_H__

#include <vector>

namespace frozenca::easy {

using namespace std;

void insertion_sort(vector<int>& A) {
    long n = ssize(A);
    for (long i = 1; i < n; ++i) {
        int key = A[i];
        long j = i - 1;
        while (j >= 0 && A[j] > key) {
            A[j + 1] = A[j];
            --j;
        }
        A[j + 1] = key;
    }
}

} // namespace frozenca::easy

#endif //__CLRS4_INSERTION_SORT_EASY_H__
