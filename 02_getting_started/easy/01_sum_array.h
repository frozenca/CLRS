#ifndef __CLRS4_SUM_ARRAY_EASY_H__
#define __CLRS4_SUM_ARRAY_EASY_H__

#include <vector>

namespace frozenca {

using namespace std;

int sum_array(const vector<int>& A) {
    long n = ssize(A);
    int sum = 0;
    for (long i = 0; i < n; ++i) {
        sum += A[i];
    }
    return sum;
}

} // namespace frozenca


#endif //__CLRS4_SUM_ARRAY_EASY_H__