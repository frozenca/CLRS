#ifndef __CLRS4_STACK_H__
#define __CLRS4_STACK_H__

#include <array>
#include <common.h>
#include <stdexcept>

namespace frozenca {

using namespace std;

template <typename T, size_t N = arr_default_length> class Stack {
  array<T, N> data;
  index_t top = 0;

public:
  [[nodiscard]] bool stack_empty() const { return top == 0; }

  void push(const T &x) {
    if (top == N) {
      throw overflow_error("overflow\n");
    }
    data[top] = x;
    top++;
  }

  T pop() {
    if (stack_empty()) {
      throw underflow_error("underflow\n");
    }
    --top;
    return data[top];
  }
};

} // namespace frozenca

#endif //__CLRS4_STACK_H__
