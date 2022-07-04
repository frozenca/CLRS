#ifndef __CLRS4_FIBONACCI_DP_H__
#define __CLRS4_FIBONACCI_DP_H__

#include <cassert>
#include <common.h>
#include <vector>


namespace frozenca {

using namespace std;

index_t fibonacci(index_t n) {
  if (n <= 1) {
    return 1;
  }
  vector<index_t> f(n + 1);
  f[0] = 1;
  f[1] = 1;
  for (index_t i = 2; i <= n; ++i) {
    f[i] = f[i - 1] + f[i - 2];
  }
  return f[n];
}

} // namespace frozenca

#endif //__CLRS4_FIBONACCI_DP_H__
