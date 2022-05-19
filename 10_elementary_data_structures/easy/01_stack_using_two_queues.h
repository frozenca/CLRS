#ifndef __CLRS4_STACK_TWO_QUEUES_H__
#define __CLRS4_STACK_TWO_QUEUES_H__

#include <common.h>
#include <queue>

namespace frozenca {

using namespace std;

template <typename T> class StackTwoQueues {
  queue<T> A;
  queue<T> B;

public:
  void push(const T &x) { A.push(x); }

  T pop() {
    while (A.size() > 1) {
      B.push(A.front());
      A.pop();
    }
    auto x = A.front();
    A.pop();
    swap(A, B);
    return x;
  }
};

} // namespace frozenca

#endif //__CLRS4_STACK_TWO_QUEUES_H__
