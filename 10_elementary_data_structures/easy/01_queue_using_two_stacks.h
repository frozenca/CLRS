#ifndef __CLRS4_QUEUE_TWO_STACKS_H__
#define __CLRS4_QUEUE_TWO_STACKS_H__

#include <common.h>
#include <stack>

namespace frozenca {

using namespace std;

template <typename T> class QueueTwoStacks {
  stack<T> A;
  stack<T> B;

public:
  void enqueue(const T &x) { A.push(x); }

  T dequeue() {
    if (B.empty()) {
      while (!A.empty()) {
        B.push(A.top());
        A.pop();
      }
    }
    auto x = B.top();
    B.pop();
    return x;
  }
};

} // namespace frozenca

#endif //__CLRS4_QUEUE_TWO_STACKS_H__
